#include "envoy/registry/registry.h"
#include "envoy/upstream/retry.h"

#include "source/common/upstream/retry_factory.h"

#include "test/mocks/server/factory_context.h"
#include "test/mocks/stream_info/mocks.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "library/common/extensions/retry/options/network_configuration/config.h"
#include "library/common/network/configurator.h"

using namespace testing;

namespace Envoy {
namespace Extensions {
namespace Retry {
namespace Options {
namespace {

TEST(NetworkConfigurationRetryOptionsPredicateTest, PredicateTest) {
  NiceMock<Server::Configuration::MockFactoryContext> mock_factory_context;
  NiceMock<Envoy::StreamInfo::MockStreamInfo> mock_stream_info;
  Upstream::RetryExtensionFactoryContextImpl retry_extension_factory_context{
      *mock_factory_context.singleton_manager_};

  auto configurator = Network::ConfiguratorFactory(mock_factory_context).get();
  ASSERT_NE(nullptr, configurator);

  auto factory = Registry::FactoryRegistry<Upstream::RetryOptionsPredicateFactory>::getFactory(
      "envoy.retry_options_predicates.network_configuration");
  ASSERT_NE(nullptr, factory);

  auto proto_config = factory->createEmptyConfigProto();
  auto predicate = factory->createOptionsPredicate(*proto_config, retry_extension_factory_context);

  ASSERT_EQ(absl::nullopt,
            predicate->updateOptions({mock_stream_info, nullptr}).new_upstream_socket_options_);
}

TEST(NetworkConfigurationRetryOptionsPredicateTest, PredicateTestWithoutConfigurator) {
  NiceMock<Server::Configuration::MockFactoryContext> mock_factory_context;
  Upstream::RetryExtensionFactoryContextImpl retry_extension_factory_context{
      *mock_factory_context.singleton_manager_};

  auto factory = Registry::FactoryRegistry<Upstream::RetryOptionsPredicateFactory>::getFactory(
      "envoy.retry_options_predicates.network_configuration");
  ASSERT_NE(nullptr, factory);

  auto proto_config = factory->createEmptyConfigProto();
  EXPECT_DEATH(factory->createOptionsPredicate(*proto_config, retry_extension_factory_context),
               "unexpected nullptr network configurator");
}

} // namespace
} // namespace Options
} // namespace Retry
} // namespace Extensions
} // namespace Envoy
