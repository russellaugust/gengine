//
// SheepCompiler.h
//
// Clark Kromenaker
//
// Capable of taking sheep in text format and compiling it to a binary SheepScript asset.
// Uses the Sheep scanner/parser generated by Flex/Bison respectively.
//
#pragma once
#include <istream>
#include <map>
#include <string>

#include "SheepScanner.h"
#include "sheep.tab.hh"

class SheepScript;

class SheepCompiler
{
public:
    SheepCompiler() = default;
    virtual ~SheepCompiler();
    
    SheepScript* Compile(const char* filePath);
    SheepScript* Compile(const std::string& name, const std::string& sheep);
    SheepScript* Compile(const std::string& name, std::istream& stream);
	
	void Warning(SheepScriptBuilder* builder, const Sheep::location& location, const std::string& message);
    void Error(SheepScriptBuilder* builder, const Sheep::location& location, const std::string& message);
	
private:
	// Scanner, generated by Flex, used to parse text to tokens.
    SheepScanner* mScanner = nullptr;
	
	// Parser, generated by Bison, used to parse tokens into language grammer.
    Sheep::Parser* mParser = nullptr;
};
