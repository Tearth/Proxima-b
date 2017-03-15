#include "Headers/Command.h"

Command::Command(ECommandType type, vector<string> args)
{
	Type = type;
	Arguments = args;
}

Command::~Command()
{

}