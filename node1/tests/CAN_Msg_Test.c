CAN_MSG msg_tx;
msg_tx.ID = 0x02;
msg_tx.DLC = 0x01;
msg_tx.Data[0] = 0x04;

CAN_Send_Message(&msg_tx);

CAN_MSG msg_rx;
CAN_Receive_Message(&msg_rx);

printf("--- NEW MESSAGE ---\n");
printf("msg_send: ID: %2X, DLC: %2X, Data: %2X\n", msg_tx.ID, msg_tx.DLC, msg_tx.Data);
printf("msg_receive: ID: %2X, DLC: %2X, Data: %2X\n", msg_rx.ID, msg_rx.DLC, msg_rx.Data);
