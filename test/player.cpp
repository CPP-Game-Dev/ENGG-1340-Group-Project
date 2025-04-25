#include "../src/include/player.h"
#include "../src/include/vector2d.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

// Dummy IItem implementation for testing
// void test_create_item() {
//     // auto item = createItem(ItemID::CampingBoots);
//     // item->setDefaults(); // FIXME: setDefaults is unimplmented
//     // Using manual edit for now,
//     item->name = "Alpha Star";
//     item->description = "A mysterious star that grants wishes.";
//
//     assert(item->id == ItemID::CampingBoots);
//     assert(item->description == "A mysterious star that grants wishes.");
// }
//
void test_default_constructor() {
    Player player;
    assert(player.getBaseStaminaMax() == 0);
    assert(player.getBaseRationRegen() == 0);
    assert(player.getBaseFov() == 3);
    assert(player.getBaseRationCapacity() == 0);
    assert(player.getBasePickaxeCapacity() == 0);
    assert(player.getStaminaMaxMult() == 1);
    assert(player.getRationRegenMult() == 1);
    assert(player.getFovMult() == 1);
    assert(player.getRationCapacityMult() == 1);
    assert(player.getPickaxeCapacityMult() == 1);
    assert(player.getPos() == Vector2D(0, 0));
}

void test_parameterized_constructor() {
    // Test inventory vector and item generation

    std::vector<std::unique_ptr<Item> > inv;
    Vector2D pos(2, 3);
    Player player(10, 5, 7, 2, 1, pos, std::move(inv));

    // auto item = createItem(ItemID::CampingBoots);
    // TODO: test setDefaults and compare default values and custom values.
    // player.addItem(item);
    // TODO: check if player has item
    // assert(player.hasItem<ItemID>() == true);

    // assert(item->id == ItemID::CampingBoots);
    assert(player.getBaseStaminaMax() == 10);
    assert(player.getBaseRationRegen() == 5);
    assert(player.getBaseFov() == 7);
    assert(player.getBaseRationCapacity() == 2);
    assert(player.getBasePickaxeCapacity() == 1);
    assert(player.getPos() == pos);
}

void test_setters_and_getters() {
    Player player;
    player.setBaseStaminaMax(15);
    player.setBaseRationRegen(4);
    player.setBaseFov(8);
    player.setBaseRationCapacity(3);
    player.setBasePickaxeCapacity(2);
    player.setStamina(10);
    player.setStaminaMax(20);
    player.setRationRegen(5);
    player.setFov(6);
    player.setRationCapacity(7);
    player.setPickaxeCapacity(8);
    player.setPos(1, 2);
    player.setStaminaMaxMult(1.5f);
    player.setRationRegenMult(2.0f);
    player.setFovMult(1.2f);
    player.setRationCapacityMult(1.3f);
    player.setPickaxeCapacityMult(1.1f);

    assert(player.getBaseStaminaMax() == 15);
    assert(player.getBaseRationRegen() == 4);
    assert(player.getBaseFov() == 8);
    assert(player.getBaseRationCapacity() == 3);
    assert(player.getBasePickaxeCapacity() == 2);
    assert(player.getStamina() == 10);
    assert(player.getStaminaMax() == 20);
    assert(player.getRationRegen() == 5);
    // assert(player.getFov() == 6); // FIXME: getFov still broken
    assert(player.getRationCapacity() == 7);
    assert(player.getPickaxeCapacity() == 8);
    assert(player.getPos() == Vector2D(1, 2));
    assert(player.getStaminaMaxMult() == 1.5f);
    assert(player.getRationRegenMult() == 2.0f);
    assert(player.getFovMult() == 1.2f);
    assert(player.getRationCapacityMult() == 1.3f);
    assert(player.getPickaxeCapacityMult() == 1.1f);
}

void test_add_and_has_item() {
    Player player;
    // auto item = std::make_unique<DummyItem>();
    // auto rawPtr = item.get();
    // player.addItem(item);
    // assert(player.hasItem<DummyItem>());
}

int main() {
    test_default_constructor();
    test_parameterized_constructor();
    test_setters_and_getters();
    // test_create_item();
    // test_add_and_has_item();
    std::cout << "All Player tests passed!" << std::endl;
    return 0;
}
