#include "CLISetupMode.hpp"

CLISetupMode::CLISetupMode() {

}

String CLISetupMode::_readfile() {
  String data;
  data.reserve(1024);
  while (1) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == 4) {
        return data;
      }
      data.concat(c);
    }
    yield();
  }
}

String CLISetupMode::_readline() {
  String line;
  line.reserve(256);
  line = "";
  while (1) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == '\b') {
        if (line.length() > 0) {
          line = line.substring(0, line.length() - 1);
          Serial.print("\b \b");
        }
      } else if (c == '\r') {
        // pass
      }  else {
        if (c == '\n') {
          Serial.println();
          return line;
        }
        Serial.print(c);
        line.concat(c);
      }
    }
    yield();
  }
}

void CLISetupMode::_ls() {
  Dir dir = SPIFFS.openDir("");
  while (dir.next()) {
    Serial.print(dir.fileName());
    Serial.print(" ");
    File f = dir.openFile("r");
    Serial.println(f.size());
  }
}

void CLISetupMode::_cat(String filename) {
  File f = SPIFFS.open(filename, "r");
  if (f) {
    String data = f.readString();
    f.close();
    Serial.print(data);
    if (!data.endsWith("\n")) {
      Serial.println();
      Serial.println("[no eol]");
    }
  } else {
    Serial.println("File not found");
  }
}

void CLISetupMode::_format() {
  if (SPIFFS.format()) {
    Serial.println("Format successful");
  } else {
    Serial.println("Format failed");
  }
}

void CLISetupMode::_rm(String filename) {
  if (SPIFFS.remove(filename)) {
    Serial.println("File removed");
  } else {
    Serial.println("File remove failed");
  }
}

void CLISetupMode::_put(String filename) {
  Serial.println("[paste data and finish with control-d]");
  String data = _readfile();
  File f = SPIFFS.open(filename, "w");
  f.print(data);
  f.close();
  Serial.print("[");
  Serial.print(data.length(), DEC);
  Serial.println(" bytes saved]");
}

void CLISetupMode::run() {
  while (1) {
    Serial.print("setup> ");
    String line = _readline();
    line.trim();

    if (line.equals("restart")) {
      ESP.restart();
    }
    
    if (line.equals("ls")) {
      _ls();
    }

    if (line.equals("format")) {
      _format();
    }

    if (line.startsWith("rm ")) {
      String filename = line.substring(3);
      filename.trim();
      _rm(filename);
    }

    if (line.startsWith("cat ")) {
      String filename = line.substring(4);
      filename.trim();
      _cat(filename);
    }

    if (line.startsWith("put ")) {
      String filename = line.substring(4);
      filename.trim();
      _put(filename);
    }

    yield();
  }
}
