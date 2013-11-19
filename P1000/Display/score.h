/*
 * score.h
 *
 * Created: 19.11.2013 13:02:55
 *  Author: mortenmj
 */ 


#ifndef SCORE_H_
#define SCORE_H_

void score_init (void);
void score_display (u8g_t *u8g);
void score_lock (void);
void score_reset (void);
void score_unlock (void);

#endif /* SCORE_H_ */