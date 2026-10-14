// Reuse the production-code fixture and its owned-memory/game-state adapters.
#define main FeatureRegressionMain
#include "feature_regressions.cpp"
#undef main

int main() {
    using namespace cs2_dumper::schemas::client_dll;
    Buffer client(cs2_dumper::offsets::client_dll::dwViewAngles + 64);
    Buffer pawn(0x5000), weapon(0x3000), services(0x1000), identity(0x50);
    Buffer system(0x100), chunk(0x70 * 512);
    constexpr uint32_t handle = 1 | (3u << 15);
    system.put(0x10, chunk.addr()); chunk.put(0x70, weapon.addr());
    weapon.put(0x10, identity.addr()); identity.put(0x10, handle);
    weapon.put(skins::OFF_ITEM_DEF_INDEX, uint16_t(skins::WEAPON_HEGRENADE));
    weapon.put(C_BaseCSGrenade::m_flThrowStrength, 1.0f);
    pawn.put(C_BasePlayerPawn::m_pWeaponServices, services.addr());
    services.put(CPlayer_WeaponServices::m_hActiveWeapon, handle);
    game_state::test_state.pawn = pawn.addr();
    game_state::test_state.entity_list = system.addr();
    sdk::ViewMatrix matrix{};
    matrix.matrix[0][0] = matrix.matrix[1][1] = matrix.matrix[3][3] = 1;
    pattern_resolver::view_matrix = reinterpret_cast<decltype(pattern_resolver::view_matrix)>(&matrix);
    static unsigned traces = 0;
    engine_trace::functions = {
        +[](engine_trace::Data*) {}, +[](engine_trace::Hit*) {},
        +[](engine_trace::Filter*, uintptr_t, uint64_t, int, int)->void* { return nullptr; },
        +[](engine_trace::Data*, sdk::Vector3, sdk::Vector3, engine_trace::Filter*, int, bool) { ++traces; },
        +[](engine_trace::Data*, engine_trace::Hit*, float, void*) {}
    };
    config::grenade::trajectory = true; config::grenade::surface_info = false;
    auto* context = ImGui::CreateContext();
    auto& io = ImGui::GetIO(); io.IniFilename = nullptr;
    io.DisplaySize = {1920, 1080}; io.DeltaTime = 1.f / 60;
    unsigned char* pixels; int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    auto render = [&] {
        ImGui::NewFrame();
        features::RenderGrenadePredictionAndSurfaceInfo(game_state::test_state, {}, client.addr());
        ImGui::Render();
    };
    render(); assert(traces == 192);
    render(); assert(traces == 192); // Stable input uses the cache.
    pawn.put(cs2_dumper::schemas::client_dll::C_BaseEntity::m_vecAbsVelocity, sdk::Vector3{100, 0, 0});
    render(); assert(traces == 192); // Reuse the path inside the 33 ms simulation budget.
    test_time += 33; render(); assert(traces == 384);
    weapon.put(C_BaseCSGrenade::m_flThrowStrength, .5f);
    render(); assert(traces == 576);
    test_time += 100; render(); assert(traces == 768);
    engine_trace::faulted = true;
    render(); assert(traces == 768 && ImGui::GetDrawData()->TotalVtxCount == 0);
    engine_trace::faulted = false;
    config::grenade::trajectory = false;
    render();
    config::grenade::trajectory = true;
    render(); // Enabling the feature must rebuild immediately.
    const auto beforeMotion = traces;
    for (int frame=0; frame<250; ++frame) {
        test_time += 4;
        client.put(cs2_dumper::offsets::client_dll::dwViewAngles, sdk::Vector3{0,float(frame),0});
        render();
    }
    const auto movingTraces = traces-beforeMotion;
    assert(movingTraces > 0 && movingTraces <= 31*192);
    config::grenade::trajectory = false; config::grenade::surface_info = true;
    const auto beforeSurface = traces;
    for (int frame=0; frame<250; ++frame) {test_time+=4;render();}
    assert(traces-beforeSurface <= 31);
    config::grenade::surface_info = false;
    std::cout << "Moving prediction: " << movingTraces << " traces over 250 frames (previously up to 48000)\n";
    // Discover and render an actual moving projectile with an unavailable
    // high-water offset and all other world/prediction features disabled.
    Buffer projectile(0x4000), projectileIdentity(0x50), scene(0x200);
    constexpr uint32_t projectileHandle = 129 | (7u << 15);
    const char projectileName[] = "hegrenade_projectile";
    chunk.put(0x70 * 129, projectile.addr());
    projectile.put(0x10, projectileIdentity.addr());
    projectileIdentity.put(0x10, projectileHandle);
    projectileIdentity.put(CEntityIdentity::m_designerName, reinterpret_cast<uintptr_t>(projectileName));
    projectile.put(cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode, scene.addr());
    game_state::test_state.in_game = true;
    config::grenade::trajectory = config::grenade::surface_info = false;
    config::grenade::thrown_trail = true;
    auto renderTrail = [&] {
        ImGui::NewFrame();
        features::UpdateWorldVisuals(game_state::test_state);
        features::RenderThrownGrenades(game_state::test_state, client.addr());
        ImGui::Render();
    };
    test_time += 100;
    scene.put(CGameSceneNode::m_vecAbsOrigin, sdk::Vector3{0,0,0});
    renderTrail();
    assert(features::thrownTrails.trails.count(projectileHandle));
    test_time += 20;
    scene.put(CGameSceneNode::m_vecAbsOrigin, sdk::Vector3{2,0,0});
    renderTrail();
    assert(features::thrownTrails.trails.at(projectileHandle).points.size() == 2);
    assert(ImGui::GetDrawData()->TotalVtxCount > 0);
    config::grenade::thrown_trail = false;
    renderTrail();
    assert(features::thrownTrails.trails.empty() && ImGui::GetDrawData()->TotalVtxCount == 0);
    ImGui::DestroyContext(context);
    grenade_trails::Cache trails;
    trails.Sample(1, {0, 0, 0}, 100);
    trails.Sample(1, {10, 0, 0}, 110);
    assert(trails.trails.at(1).points.size() == 1);
    trails.Sample(1, {10, 0, 0}, 120);
    trails.Sample(1, {10, 0, 0}, 140);
    assert(trails.trails.at(1).points.size() == 2);
    trails.Sample(1 | (1u << 15), {20, 0, 0}, 140);
    assert(trails.trails.size() == 2);
    trails.Prune(2140);
    assert(trails.trails.size() == 2);
    trails.Prune(2141);
    assert(trails.trails.empty());
    for (unsigned i = 0; i < 300; ++i) trails.Sample(1, {float(i * 2), 0, 0}, i * 20);
    assert(trails.trails.at(1).points.size() == 256);
    std::cout << "Grenade regressions passed\n";
}
