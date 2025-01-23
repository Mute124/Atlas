#include <gmock/gmock.h>
#include <gtest/gtest.h>
/*#include "engine"
TEST_CASE("getComponent") {
    // Test that getComponent returns a valid component when it exists
    {
        Atlas::Component component;
        std::shared_ptr<Atlas::IGameObject> owner = std::make_shared<Atlas::IGameObject>();
        component.mOwner = owner;
        component.mComponents["TestComponent"] = std::make_shared<TestComponent>();
        auto result = component.getComponent<TestComponent>();
        REQUIRE(result != nullptr);
        REQUIRE(std::dynamic_pointer_cast<TestComponent>(result) != nullptr);
    }
    // Test that getComponent returns nullptr when the component does not exist
    {
        Atlas::Component component;
        std::shared_ptr<Atlas::IGameObject> owner = std::make_shared<Atlas::IGameObject>();
        component.mOwner = owner;
        auto result = component.getComponent<TestComponent>();
        REQUIRE(result == nullptr);
    }
    // Test that getComponent throws an exception when the component type is not registered
    {
        Atlas::Component component;
        std::shared_ptr<Atlas::IGameObject> owner = std::make_shared<Atlas::IGameObject>();
        component.mOwner = owner;
        REQUIRE_THROWS_AS(component.getComponent<UnregisteredComponent>(), std::runtime_error);
    }
    // Test that getComponent returns the correct component type
    {
        Atlas::Component component;
        std::shared_ptr<Atlas::IGameObject> owner = std::make_shared<Atlas::IGameObject>();
        component.mOwner = owner;
        component.mComponents["TestComponent"] = std::make_shared<TestComponent>();
        auto result = component.getComponent<TestComponent>();
        REQUIRE(std::dynamic_pointer_cast<TestComponent>(result) != nullptr);
        REQUIRE(std::dynamic_pointer_cast<OtherComponent>(result) == nullptr);
    }
}
class TestComponent : public Atlas::Component {
};
class OtherComponent : public Atlas::Component {
};
class UnregisteredComponent : public Atlas::Component {
};
*/

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
