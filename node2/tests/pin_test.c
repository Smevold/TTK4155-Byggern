// BEGIN PIN B13 OUTPUT TEST
PIOB->PIO_WPMR = 0; // Clear WPEN bit to disable write protect

PIOB->PIO_PER |= PIO_PB13; // Enable register pin 13
PIOB->PIO_OER |= PIO_PB13; // Output enable register pin 13
PIOB->PIO_SODR |= PIO_PB13; // Set output data register pin 13
// END PIN B13 OUTPUT TEST