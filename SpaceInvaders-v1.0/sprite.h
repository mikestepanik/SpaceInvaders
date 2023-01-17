#ifndef sprite_h
#define sprite_h
//                                     Images
//
// It seems that images are saved in reverse; i.e. top row is actually the bottom row of pixels.
//
// See end of Section 15.3.2. Buffer-based Graphics on the Nokia 5110 discussing Figure 15.7.
// on web page: Chapter 15: Complete Embedded System, Modified to be compatible with EE319K Lab 10
// http://users.ece.utexas.edu/~valvano/Volume1/E-Book/C15_CompleteEmbeddedSystem.htm
// The 2-D image is stored in row-major format. The image is stored up-side down. 
// When plotting it on the screen the Nokia5110_PrintBMP() function will reverse it so it is seen right-side up.

// Extern -> extend visibility of a function throughout the whole program

// Black Enemy is all zeros.
extern const unsigned short BlackEnemy[160];
// Black Laser is all zeros.
extern const unsigned short BlackLaser[36];
extern const unsigned short SmallEnemy10pointA[];
extern const unsigned short SmallEnemy10pointB[];
extern const unsigned short MedEnemy20pointA[];
extern const unsigned short MedEnemy20pointB[];
extern const unsigned short LargeEnemy30pointA[];
extern const unsigned short LargeEnemy30pointB[];
extern const unsigned short losestreaks[];
extern const unsigned short PlayerShip0[];
extern const unsigned short PlayerShip1[];
extern const unsigned short Bunker0[];
extern const unsigned short Bunker1[];
extern const unsigned short Bunker2[];
extern const unsigned short Bunker3[];
extern const unsigned short EnemyLaser0[];
extern const unsigned short Laser0[];
extern const unsigned short Laser1[];
extern const unsigned short Laser2[];
extern const unsigned short BigExplosion0[];
extern const unsigned short BigExplosion1[];
extern const unsigned short SmallExplosion0[];
extern const unsigned short SmallExplosion1[];
extern const unsigned short Missile0[];
extern const unsigned short Missile1[];
extern const unsigned short Missile2[];
#endif 
