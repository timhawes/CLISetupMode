#ifndef APP_CLISETUPMODE_HPP
#define APP_CLISETUPMODE_HPP

#include <Arduino.h>
#include <FS.h>

class CLISetupMode
{
private:
  String _readfile();
  String _readline();
  void _cat(String filename);
  void _format();
  void _ls();
  void _put(String filename);
  void _rm(String filename);
public:
  CLISetupMode();
  void run();
};

#endif
