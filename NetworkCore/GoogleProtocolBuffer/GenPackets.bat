protoc.exe --proto_path=./Protocol --cpp_out=./Protocol ./Protocol/*.proto
IF ERRORLEVEL 1 PAUSE