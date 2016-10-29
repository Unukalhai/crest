/*!
 * \file crest.h
 * \author Unukalhai
 * ------------------------------------------------------------------------
 */

#include "crest.h"

#include <QCoreApplication>

#include <gtest/gtest.h>

#include <vector>
#include <list>

class CrestTest : public ::testing::Test
{
public:
  Crest * crest;

  virtual void SetUp()
  {
    crest = new Crest();
  }

  virtual void TearDown()
  {
    delete crest;
  }
};

TEST_F(CrestTest, getName)
{
  ASSERT_EQ("Drake", crest->getName(24698));
}

TEST_F(CrestTest, getMarketPrice_4Capital)
{
  ASSERT_EQ(1850082011, crest->getMarketPrice(19722, 10000002 , "sell"));
}

TEST_F(CrestTest, getMarketPrice_4SubCapital)
{
  ASSERT_EQ(54499700, crest->getMarketPrice(24698, 10000002 , "sell", 60003760));
}

TEST_F(CrestTest, getInsurance)
{
  vector<int> insurance_id = {24698, 19722};
  list<Crest::insurance> insurances = crest->getInsurance(insurance_id.data(), insurance_id.size(), "Platinum");
  ASSERT_EQ(497456678, insurances.front().cost);
  ASSERT_EQ(16450238, insurances.back().cost);
}

int main(int argc, char** argv)
{
  QCoreApplication a(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
