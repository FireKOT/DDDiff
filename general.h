#ifndef _GENERAL_H
#define _GENERAL_H


//! @brief compare numbers of double type with EPS accuracy
//!
//! @param [in] n first double number
//! @param [in] m second double number
//!
//! @return [int] result of compare
//!
//! @retval true  if numbers are equal
//! @retval false otherwise
int  IsEqDoubleNums     (double n, double m);


//! @brief clear current line in console
//!
//! @return [int] result of cleaning
//!
//! @retval true  if there were trash in line
//! @retval false otherwise
int  ClearBuf    ();


//! @brief request permission to continue
int WantContinue ();
;
int   GetFileSize (const char *file_name);
char *GetText     (const char *file_name);
void  FreeText    (char *text);


#endif
