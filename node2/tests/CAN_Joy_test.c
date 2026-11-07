SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    uart_init(84000000, 9600);

    // Can initlialize struct
    CanInit init;
    init.phase2 = 6;
    init.propag = 0;
    init.phase1 = 6;
    init.sjw = 0;
    init.brp = 41; // brp provided by Claude, because we misunderstood
    // BRP+1 = MCK / (bitrate * 16) = 84e6 / (125e3 * 16) = 42
    init.smp = 1;

    can_init(init, 0);
    
    CanMsg msg_tx; 
    CanMsg msg_rx;
    
    Timer_Init();

    PWM_Init();

    while (1)
    {
        // --- CAN JOY TEST BEGIN ---

        // Only update tx and send, if received
        if(can_rx(&msg_rx)) {

        // Send same msg back
        msg_tx.id = msg_rx.id; 
        msg_tx.length = msg_rx.length; 
        msg_tx.byte[0] = msg_rx.byte[0]; // Set value first byte
        msg_tx.byte[1] = msg_rx.byte[1];
        msg_tx.byte[2] = msg_rx.byte[2];
        msg_tx.byte[3] = msg_rx.byte[3];
        msg_tx.byte[4] = msg_rx.byte[4];
        msg_tx.byte[5] = msg_rx.byte[5];
        msg_tx.byte[6] = msg_rx.byte[6];

        can_tx(msg_tx);

        printf("--- NEW MESSAGE ---\n");
        printf("msg_receive: ID: %2X, DLC: %2X, Data: %2X, %2X, %2X, %2X, %2X, %2X, %2X\n", msg_rx.id, msg_rx.length, msg_tx.byte[0], msg_tx.byte[1], msg_tx.byte[2], msg_tx.byte[3], msg_tx.byte[4], msg_tx.byte[5], msg_tx.byte[6]);
        printf("msg_send: ID: %2X, DLC: %2X, Data: %2X, %2X, %2X, %2X, %2X, %2X, %2X\n", msg_tx.id, msg_tx.length, msg_tx.byte[0], msg_tx.byte[1], msg_tx.byte[2], msg_tx.byte[3], msg_tx.byte[4], msg_tx.byte[5], msg_tx.byte[6]);
        }

        // --- CAN JOY TEST END ---