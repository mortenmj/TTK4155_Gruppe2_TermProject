/**
 * @file
 *
 * @ingroup Score
 *
 * @brief Score sub-module
 */

/*
 * score.h
 *
 * Created: 19.11.2013 13:02:55
 *  Author: mortenmj
 */


#ifndef SCORE_H_
#define SCORE_H_

/**
 * @defgroup Score Score sub-module
 *
 * @ingroup Display
 *
 * @brief Scorekeeping for the game
 *
 * @{
 */

/**
 * @brief Initialize scorekeeping
 */

/**
 * @brief Initialize menu
 */
void score_init (void);

/**
 * @brief Function to display the score
 *
 * @param u8g The display device
 */
void score_display (u8g_t *u8g);

/**
 * @brief Function to display the highscore screen
 *
 * @param u8g The display device
 */
void score_hiscore_display (u8g_t *u8g);

/**
 * @brief Start the score counter
 */
void score_start (void);

/**
 * @brief Reset the score counter
 */
void score_reset (void);

/**
 * @brief Save highscore to persistent memory
 */
void score_save (void);

/**
 * @brief Clear highscore from persistent memory
 */
void score_clear (void);

/** @} */

#endif /* SCORE_H_ */