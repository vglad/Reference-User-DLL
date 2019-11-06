#include "pch.h"
#include <memory>
#include <iterator>
#include <string>
#include <fstream>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <vector>
#include <iostream>
#include <chrono>

#pragma once
//#include "..\User_DLL\user.h"
#include "..\User_DLL\MagicNumbers.h"
#include "..\User_DLL\Converter.h"
//#include "..\User_DLL\IBridge.h"
//#include "..\User_DLL\OHCommunicator.h"
//#include "FakeOHCommunicator.h"
//#include "..\User_DLL\TableInfo.h"
//#include "..\User_DLL\Player.h"
//#include "..\User_DLL\Range.h"
//#include "..\User_DLL\RangeOrder.h"
//#include "..\User_DLL\Update.h"
//#include "..\User_DLL\Table.h"
//#include "..\User_DLL\BotLogic.h"

//using namespace std;

TEST(Converter_stack_to_blinds, ReturnProperValuesInBB) {
    Converter conv;

    EXPECT_EQ(0, conv.stack_to_blinds(0, 4));

}
