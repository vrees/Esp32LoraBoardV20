function decodeUplink(input) {
  var bytes = input.bytes;
  var decoded = {};
  var heartBeat = {};

  wasserstand = bytes[0];
  
  batterieSpannung = (bytes[1] * 256 + bytes[2]) / 100 ;
  
  operationMode = bytes[3];
  
  decoded['Wasserstand'] = wasserstand
  decoded['BatterieSpannung'] = batterieSpannung

  switch (operationMode) {
    case 1:
      decoded['WakeupBy'] = 1 // 'Timer'
      break;
    case 2:
      decoded['WakeupBy'] = 2 // 'Sensor'
      break;
    case 3:
      decoded['WakeupBy'] = 3 // 'Debug'
      break;
  }
  
    bootCount = (bytes[4] * 256 + bytes[5]);
    execTooLongCount = (bytes[6] * 256 + bytes[7]);
    
    heartBeat = bootCount % 2;
    
  decoded['BootCount'] = bootCount;
  decoded['HeartBeat'] = heartBeat;
  decoded['ExecutionTooLongCount'] = execTooLongCount;
  
  return {
    data: decoded
  };
}