#include "skybox.h"
#include "config.h"
#include "game_state.h"
#include "../sdk/entity.h"
#include <string>
#include <unordered_map>

namespace skybox {
ScopedColor::ScopedColor(uintptr_t draw_data, int count) {
    if (!draw_data || count <= 0) return;
    float tint[3]{};
    bool useOriginal = false;
    float night = 1;
    {
        std::unique_lock<std::recursive_mutex> lock(config::mutex, std::try_to_lock);
        if (!lock.owns_lock() || (!config::skybox::color_enabled && !config::lighting::night_mode)) return;
        useOriginal = !config::skybox::color_enabled;
        if (config::lighting::night_mode && std::isfinite(config::lighting::night_brightness)) night = std::clamp(config::lighting::night_brightness, .02f, 1.0f);
        for (int i = 0; i < 3; ++i)
            tint[i] = config::skybox::color[i] * config::skybox::brightness;
    }
    if (!game_state::GetSnapshot().in_game) return;
    // Supplied DrawArray layout: scene object at +0x18, RGB floats at +0xE8.
    const auto object = sdk::read_value<uintptr_t>(draw_data + 0x18);
    if (!object) return;
    const auto address = object + 0xE8;
    if (!sdk::read_memory(address, original_)) return;
    for (int i = 0; i < 3; ++i) {
        tint[i] = (useOriginal ? original_[i] : tint[i]) * night;
        if (!std::isfinite(tint[i])) return;
    }
    if (sdk::write_memory(address, tint)) address_ = address;
}
ScopedColor::~ScopedColor() {
    if (address_) sdk::write_memory(address_, original_);
}
namespace {
    using UpdateSkybox = void (__fastcall*)(void*);
    using FindMaterial = void* (__fastcall*)(void*, void***, const char*);
    void* material_system = nullptr;
    UpdateSkybox update_skybox = nullptr;
    FindMaterial find_material = nullptr;
    std::unordered_map<std::string, void**> materials;
    std::string applied_path;
    uintptr_t last_system = 0;
    unsigned applied_revision = 0;
    ULONGLONG next_attempt = 0;
    std::string attempted_path;
    uintptr_t attempted_system = 0;
    unsigned attempted_revision = 0;
    std::atomic<bool> restoring{false}, restored{true};
    struct AppliedEntity {
        uintptr_t identity;
        uint32_t handle;
        void** map_material;
    };
    std::unordered_map<uintptr_t, AppliedEntity> applied_entities;

    void* Capture(const char* module, const char* name) {
        auto handle = GetModuleHandleA(module);
        auto factory = handle ? reinterpret_cast<void* (*)(const char*, int*)>(
            GetProcAddress(handle, "CreateInterface")) : nullptr;
        return factory ? factory(name, nullptr) : nullptr;
    }

    template<class T> T Virtual(void* object, unsigned index) {
        auto table = sdk::read_value<uintptr_t>(reinterpret_cast<uintptr_t>(object));
        auto address = table ? sdk::read_value<uintptr_t>(table + index * sizeof(void*)) : 0;
        MEMORY_BASIC_INFORMATION info{};
        if (!address || !VirtualQuery(reinterpret_cast<void*>(address), &info, sizeof(info)) ||
            info.State != MEM_COMMIT || (info.Protect & (PAGE_GUARD | PAGE_NOACCESS)) ||
            !(info.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))) return nullptr;
        return reinterpret_cast<T>(address);
    }

    bool MaterialMatches(void** handle, const std::string& requested) {
        auto material = sdk::read_value<void*>(reinterpret_cast<uintptr_t>(handle));
        using GetName = const char* (__fastcall*)(void*);
        auto get_name = Virtual<GetName>(material, 0);
        auto name = get_name ? get_name(material) : nullptr;
        if (!name) return false;
        // The material system can return a valid error material for a missing resource.
        for (size_t i = 0; i <= requested.size(); ++i) {
            char character = 0;
            if (!sdk::read_memory(reinterpret_cast<uintptr_t>(name) + i, character) ||
                character != requested.c_str()[i]) return false;
        }
        return true;
    }

    bool Apply(uintptr_t system, void** custom, bool force) {
        bool found = false, complete = true;
        std::unordered_map<uintptr_t, AppliedEntity> current_entities;
        // Poll entity identities at a throttled interval. Refresh only new/changed skies.
        for (int index = 0; index < 0x7FFF; ++index) {
            auto entity = sdk::entity_at(system, index);
            if (!entity) continue;
            auto identity = sdk::read_value<uintptr_t>(entity + sdk::off::CEntityInstance::m_pEntity);
            if (!identity) continue;
            auto name = sdk::read_value<uintptr_t>(identity + sdk::off::CEntityIdentity::m_designerName);
            char designer[8]{};
            if (!sdk::read_memory(name, designer) || std::memcmp(designer, "env_sky", 8)) continue;
            found = true;
            uint32_t handle = UINT32_MAX;
            const auto slot = entity + sdk::off::C_EnvSky::m_hSkyMaterial;
            auto map_material = sdk::read_value<void**>(slot);
            if (!sdk::read_memory(identity + 0x10, handle) || !map_material ||
                !sdk::read_value<void*>(reinterpret_cast<uintptr_t>(map_material))) {
                complete = false;
                continue;
            }
            const auto previous = applied_entities.find(entity);
            const bool already_applied = previous != applied_entities.end() &&
                previous->second.identity == identity && previous->second.handle == handle &&
                previous->second.map_material == map_material;
            if (custom) {
                if (force || !already_applied) {
                    // Method 2: actually write the entity field, then refresh its scene sky.
                    // Restore the field immediately so the entity keeps ownership of its map
                    // material. UpdateSkybox copies the selected material into the scene.
                    if (!sdk::write_memory(slot, custom)) { complete = false; continue; }
                    update_skybox(reinterpret_cast<void*>(entity));
                    if (!sdk::write_memory(slot, map_material)) {
                        status = "Skybox material field restoration failed";
                        complete = false;
                    }
                    restored = false;
                }
                current_entities.emplace(entity, AppliedEntity{identity, handle, map_material});
            } else if (already_applied) {
                // Entity field already contains its original material; refresh the scene.
                update_skybox(reinterpret_cast<void*>(entity));
            }
        }
        applied_entities = std::move(current_entities);
        return complete && (found || !custom);
    }
}

bool Initialize() {
    material_system = Capture("materialsystem2.dll", "VMaterialSystem2_001");
    find_material = Virtual<FindMaterial>(material_system, 14);
    // https://www.cspatterns.dev/json : client.dll / updateskybox
    update_skybox = reinterpret_cast<UpdateSkybox>(sdk::find_pattern("client.dll",
        "48 89 5C 24 ? 57 48 83 EC ? 48 8B F9 E8 ? ? ? ? 48 8B 47"));
    if (!find_material || !update_skybox) {
        status = "Unavailable: material system or UpdateSkybox did not resolve";
        return false;
    }
    status = "Default skybox";
    return true;
}

void OnFrameStage(int stage) {
    if (stage != 6 || !update_skybox) return;
    std::lock_guard<std::recursive_mutex> lock(config::mutex);
    const auto snapshot = game_state::GetSnapshot();
    if (!snapshot.in_game || !snapshot.entity_list) {
        last_system = 0;
        applied_path.clear();
        materials.clear();
        applied_entities.clear();
        next_attempt = 0;
        restored = true;
        return;
    }
    std::string requested = !restoring && config::skybox::enabled ? config::skybox::material : "";
    if (requested.size() >= 2 && requested.compare(requested.size() - 2, 2, "_c") == 0)
        requested.resize(requested.size() - 2);
    const auto revision = world_revision.load();
    const bool changed = snapshot.entity_list != last_system || requested != applied_path || revision != applied_revision;
    if (requested.empty() && restored) {
        applied_path.clear();
        last_system = snapshot.entity_list;
        applied_revision = revision;
        status = "Default skybox";
        return;
    }
    const bool new_request = requested != attempted_path || revision != attempted_revision ||
        snapshot.entity_list != attempted_system;
    if (!restoring && !new_request && GetTickCount64() < next_attempt) return;
    next_attempt = GetTickCount64() + 1000;
    attempted_path = requested;
    attempted_revision = revision;
    attempted_system = snapshot.entity_list;
    if (snapshot.entity_list != last_system) {
        materials.clear();
        applied_entities.clear();
    }
    void** material = nullptr;
    if (!requested.empty()) {
        if (requested.find("..") != std::string::npos || requested.find(':') != std::string::npos ||
            requested.size() < 5 || requested.compare(requested.size() - 5, 5, ".vmat")) {
            status = "Enter a game-relative .vmat material path";
            return;
        }
        auto cached = materials.find(requested);
        if (cached != materials.end()) material = cached->second;
        else {
            // Engine resource lookup; absent/unloaded resources must not replace the map sky.
            find_material(material_system, &material, requested.c_str());
            if (material && MaterialMatches(material, requested))
                materials.emplace(requested, material);
        }
        if (!material || !MaterialMatches(material, requested)) {
            status = "Material unavailable; load the skybox resource and retry";
            return;
        }
    }
    if (!Apply(snapshot.entity_list, material, changed)) {
        status = "Waiting for skybox entities / material update";
        return;
    }
    applied_path = requested;
    applied_revision = revision;
    last_system = snapshot.entity_list;
    restored = requested.empty();
    status = material ? "Custom skybox applied" : "Default skybox";
}

void RestoreBeforeUnload() {
    restoring = true;
    // Restore on the game thread while FrameStageNotify is still hooked.
    const auto deadline = GetTickCount64() + 1500;
    while (!restored && update_skybox && GetTickCount64() < deadline) Sleep(1);
}
}
