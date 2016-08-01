/*!
 * \file crest.h
 * \author Unukalhai
 * ------------------------------------------------------------------------
 */

#include <QCoreApplication>

#include "crest.h"

#include <gtest/gtest.h>

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

TEST_F(CrestTest, AddPoint)
{
  //ASSERT_NO_THROW(pb->addPoint(0.1, 10, 20));
}

int main(int argc, char** argv)
{
  QCoreApplication a(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
