#include "../manager/core/skybox.cpp"
#include <array>
#include <cassert>
#include <iostream>

namespace {
    game_state::Snapshot snapshot;
    unsigned changes = 0, lookups = 0;
    void** last_material = nullptr;
    bool missing = false;
    constexpr char custom_path[] = "materials/skybox/test.vmat";
    const char* __fastcall MaterialName(void*) {
        return missing ? "materials/dev/error.vmat" : custom_path;
    }
    void* material_vtable[] = {reinterpret_cast<void*>(&MaterialName)};
    void** material_object = material_vtable;
    void* material_handle = &material_object;
    void* __fastcall Find(void*, void*** out, const char*) {
        ++lookups;
        *out = &material_handle;
        return out;
    }
    void __fastcall Update(void* entity) {
        ++changes;
        last_material = sdk::read_value<void**>(reinterpret_cast<uintptr_t>(entity) +
            sdk::off::C_EnvSky::m_hSkyMaterial);
    }
    template<class T, size_t N> void Put(std::array<unsigned char, N>& memory, size_t offset, T value) {
        assert(offset + sizeof(value) <= N);
        std::memcpy(memory.data() + offset, &value, sizeof(value));
    }
}
namespace game_state { Snapshot GetSnapshot() { return snapshot; } }
namespace sdk {
    std::uint8_t* find_pattern(const char*, const char*) { return nullptr; }
    std::uint8_t* resolve_absolute_rip_address(std::uint8_t*, std::size_t, std::size_t) { return nullptr; }
}

int main() {
    std::array<unsigned char, 0x100> scene{};
    std::array<unsigned char, 0x20> draw{};
    const std::array<float, 3> original{0.4f, 0.5f, 0.6f};
    Put(scene, 0xE8, original);
    Put(scene, 0xF4, 123.0f); // The fourth float must remain untouched.
    Put(draw, 0x18, scene.data());
    snapshot.in_game = true;
    config::skybox::color_enabled = true;
    config::skybox::color[0] = 0.25f;
    config::skybox::color[1] = 0.5f;
    config::skybox::color[2] = 1.0f;
    const auto color_address = reinterpret_cast<uintptr_t>(scene.data()) + 0xE8;
    {
        skybox::ScopedColor tint(reinterpret_cast<uintptr_t>(draw.data()), 1);
        const auto actual = sdk::read_value<std::array<float, 3>>(color_address);
        assert((actual == std::array<float, 3>{0.75f, 1.5f, 3.0f}));
        assert(sdk::read_value<float>(color_address + 12) == 123.0f);
    }
    assert((sdk::read_value<std::array<float, 3>>(color_address) == original));
    for (int mode = 0; mode < 5; ++mode) {
        config::skybox::color_enabled = mode != 0;
        snapshot.in_game = mode != 1;
        skybox::ScopedColor tint(mode == 2 ? 0 : reinterpret_cast<uintptr_t>(draw.data()),
            mode == 3 ? 0 : mode == 4 ? -1 : 1);
        assert((sdk::read_value<std::array<float, 3>>(color_address) == original));
    }
    config::skybox::color_enabled = false;
    config::lighting::night_mode = true; config::lighting::night_brightness = .5f;
    {
        skybox::ScopedColor night(reinterpret_cast<uintptr_t>(draw.data()), 1);
        const auto actual = sdk::read_value<std::array<float, 3>>(color_address);
        assert((actual == std::array<float, 3>{.2f,.25f,.3f}));
    }
    assert((sdk::read_value<std::array<float, 3>>(color_address) == original));
    config::lighting::night_mode = false;
    std::array<unsigned char, 0x10 + 64 * 8> system{};
    std::array<unsigned char, 512 * 0x70> chunk{};
    std::array<unsigned char, sdk::off::C_EnvSky::m_hSkyMaterial + 8> entity{};
    std::array<unsigned char, 0x80> identity{};
    static char designer[] = "env_sky";
    void* default_object = &identity;
    void** default_handle = &default_object;
    Put(system, 0x10, chunk.data());
    Put(chunk, 0x70, entity.data());
    Put(entity, sdk::off::CEntityInstance::m_pEntity, identity.data());
    Put(identity, sdk::off::CEntityIdentity::m_designerName, designer);
    Put(entity, sdk::off::C_EnvSky::m_hSkyMaterial, default_handle);
    snapshot.in_game = true;
    snapshot.entity_list = reinterpret_cast<uintptr_t>(system.data());
    skybox::find_material = &Find;
    skybox::update_skybox = &Update;
    config::skybox::enabled = true;
    strcpy_s(config::skybox::material, custom_path);

    skybox::OnFrameStage(0);
    assert(changes == 0 && lookups == 0);
    skybox::OnFrameStage(6);
    assert(changes == 1 && lookups == 1 && last_material == &material_handle);
    for (int i = 0; i < 10; ++i) skybox::OnFrameStage(6);
    assert(changes == 1 && lookups == 1);
    assert(sdk::read_value<void**>(reinterpret_cast<uintptr_t>(entity.data()) +
        sdk::off::C_EnvSky::m_hSkyMaterial) == default_handle);

    ++skybox::world_revision;
    skybox::next_attempt = 0;
    skybox::OnFrameStage(6);
    assert(changes == 2 && lookups == 1);
    skybox::next_attempt = 0;
    skybox::OnFrameStage(6);
    assert(changes == 2); // Unchanged entity polling must not refresh the sky.
    Put(identity, 0x10, uint32_t{0x8001});
    skybox::next_attempt = 0;
    skybox::OnFrameStage(6);
    assert(changes == 3 && last_material == &material_handle); // Reused entity address.

    config::skybox::enabled = false;
    skybox::next_attempt = 0;
    skybox::OnFrameStage(6);
    assert(changes == 4 && last_material == default_handle);
    skybox::OnFrameStage(6);
    assert(changes == 4);

    missing = true;
    config::skybox::enabled = true;
    ++skybox::world_revision;
    skybox::next_attempt = 0;
    skybox::OnFrameStage(6);
    assert(changes == 4); // Valid error material must not replace the sky.
    const auto failed_lookups = lookups;
    skybox::OnFrameStage(6);
    assert(lookups == failed_lookups); // Failed requests are throttled too.

    snapshot.in_game = false;
    skybox::OnFrameStage(6);
    assert(skybox::materials.empty() && skybox::applied_entities.empty() && skybox::restored);
    std::cout << "Skybox regressions passed\n";
}
