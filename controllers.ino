
OutputData mooring(int dt, SensorData sensors) {
    OutputData output;
    output.e = 0;
    output.motorL = 0;
    output.motorR = 0;
    output.motorS = 0;
    return output;
}

OutputData parallelTof(int dt, SensorData sensors) {
    OutputData output;
    output.e = 0;
    output.motorL = 0;
    output.motorR = 0;
    output.motorS = 0;
    return output;
}

OutputData parallelGyro(int dt, SensorData sensors) {
    OutputData output;
    output.e = 0;
    output.motorL = 0;
    output.motorR = 0;
    output.motorS = 0;
    return output;
}

OutputData parallelForward(int dt, SensorData sensors) {
    OutputData output;
    output.e = 0;
    output.motorL = 0;
    output.motorR = 0;
    output.motorS = 0;
    return output;
}

OutputData forwardDistance(int dt, SensorData sensors) {
    OutputData output;
    output.e = 0;
    output.motorL = 0;
    output.motorR = 0;
    output.motorS = 0;
    return output;
}

OutputData rotate(int dt, SensorData sensors) {
    OutputData output;
    output.e = 0;
    output.motorL = 0;
    output.motorR = 0;
    output.motorS = 0;
    return output;
}

OutputData aruco(int dt) {
  isConnected = initSerialConnection();

  if (isConnected) {
    PositionData posData = askPosition();

    error_x_previous = error_x;
    error_y_previous = error_y;
    error_rot_previous = error_rot;

    error_x = posData.X;
    error_y = posData.Y;
    error_rot = posData.B;

    avg_error = (error_x + error_y + error_rot)/3;

    error_x_delta = error_x - error_x_previous;
    error_y_delta = error_y - error_y_previous;
    error_rot_delta = error_rot - error_rot_previous;

    error_x_sum += error_x * dt;
    error_y_sum += error_y * dt;
    error_rot_sum += error_rot *dt;

    resulting_force_x = Kp_x * error_x + Kd_x * error_x_delta / dt + Ki_x * error_x_sum;
    resulting_force_y = Kp_y * error_y + Kd_y * error_y_delta / dt + Ki_y * error_y_sum;
    resulting_moment = heli_moment + Kp_rot * error_rot + Kd_rot * error_rot_delta / dt + Ki_rot * error_rot_sum;

    
    output.motorL = ((resulting_force_x / 2) - (resulting_moment / d));
    output.motorR = ((resulting_force_x / 2) + (resulting_moment / d));
    output.motorS = resulting_force_y;
    output.e = avg_error;

    Serial.print("X: "); Serial.print(posData.X);
    Serial.print(" Y: "); Serial.print(posData.Y);
    Serial.print(" B: "); Serial.println(posData.B);
  } 
  Serial.print("ML: "); Serial.print(output.motorL);
  Serial.print(" MR: "); Serial.print(output.motorR);
  Serial.print(" MS: "); Serial.println(output.motorS);
  Serial.println("\n\n\n\n\n");
  return output;
}
