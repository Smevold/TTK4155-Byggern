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

// Can msg struct for transponding
CanMsg m_tx; 
m_tx.id = 2; // Random id
m_tx.length = 1; // Send one byte
m_tx.byte[0] = 0x04; // Set value first byte

// Can msg struct for receiving, should be fine empty?
CanMsg m_rx; 

while (1)
{
    can_tx(m_tx);
    can_rx(&m_rx);

    printf("TX id: %2X, length: %2X, byte: %2X\n RX: id: %2X, length: %2X, byte: %2X\n", m_tx.id, m_tx.length, m_tx.byte[0], m_rx.id, m_rx.length, m_rx.byte[0]);
}