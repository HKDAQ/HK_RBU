#include "Factory.h"
#include "Unity.h"

Tool* Factory(std::string tool){
Tool* ret=0;

// if (tool=="Type") tool=new Type;
if (tool=="DummyTool") ret=new DummyTool;

if (tool=="DataReceiver") ret=new DataReceiver;
  if (tool=="JobManager") ret=new JobManager;
return ret;
}

