function decodeUplink(input) {
    var bytes = input.bytes;
    var decoded = {};
    var heartBeat = {};
  
    wasserstandStatus = bytes[0];
    
    solarSpannung = (bytes[1] * 256 + bytes[2]) / 100 ;
    powerPathSpannung = (bytes[3] * 256 + bytes[4]) / 100 ;
    vcc2Spannung = (bytes[5] * 256 + bytes[6]) / 100 ;
    
    operationMode = bytes[7];
    
    decoded['WasserstandStatus'] = wasserstandStatus
    decoded['SolarSpannung'] = solarSpannung
    decoded['PowerPathSpannung'] = powerPathSpannung
    decoded['VCC2Spannung'] = vcc2Spannung
  
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
    
      bootCount = (bytes[8] * 256 + bytes[9]);
      execTooLongCount = (bytes[10] * 256 + bytes[11]);
      wasserstand = 250 - (bytes[12] * 256 + bytes[13]);  // Korrektur Abstand zwischen Senor und Wasserobefläche 
      
      heartBeat = bootCount % 2;
      
    decoded['BootCount'] = bootCount;
    // decoded['HeartBeat'] = heartBeat;
    decoded['ExecutionTooLongCount'] = execTooLongCount;
    decoded['Wasserstand'] = wasserstand;
    
    return {
      data: decoded
    };
  }