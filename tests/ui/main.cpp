#include <gtest/gtest.h>

#include <QApplication>

int main(int argc, char** argv)
{
    int          qtArgc = 0;
    QApplication app(qtArgc, nullptr);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
