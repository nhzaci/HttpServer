#include "../src/Request.hxx"

#include <gtest/gtest.h>

class HttpTest : public ::testing::Test {
protected:
  void SetUp() override{};

  std::string sampleHttpRequest =
      "GET /index.html HTTP/1.1\r\n"
      "Host: localhost\r\n"
      "Connection: keep-alive\r\n"
      "sec-ch-ua-mobile: ?0\r\n"
      "Upgrade-Insecure-Requests: 1\r\n"
      "Accept-Encoding: gzip,deflate,br\r\n"
      "Accept-Language: en-US,en;q=0.9,zh-CN;q=0.8,zh;q=0.7\r\n\r\n"
      "Some body text here for good measure";

  const char *cStr = sampleHttpRequest.c_str();
};

TEST_F(HttpTest, ParsesGetRequest) {
  auto req = http::Request::from(sampleHttpRequest);
  EXPECT_EQ(req.method(), http::Request::Method::Get);
  EXPECT_EQ(req.route(), "/index.html");
  EXPECT_EQ(req.body(), "Some body text here for good measure");
}