/***************************************** 
** File: nqueens.c
** Author: Marie desJardins
** Date: 9/12/06
** E-Mail: mariedj@cs.umbc.edu 
** 
** Prints out a legal solution to the N-queens problem,
** if one can be found without backtracking.  N is defined
** as a constant, and can be changed before compiling to
** generate different size boards.
*****************************************/ 


#define N 1000                /* The number of queens to place on the board. */
#define TRUE 1
#define FALSE 0

int Legal_Pos();

int main () {
  int rows[N];          /* Each row holds an integer that indicates the
			   column position of that queen */
  int row, col;         /* Loop indices */
  int found;            /* A flag used to terminate the loop */
  
  /* Initialize all positions to -1 (no queen placed in this row yet) */
  for ( row=0 ; row < N ; row++ ) {
    rows[row] = -1;
  }

  /* Find a legal position in each row.  No error checking if there
     is no legal position!  As soon as we find a legal position,
     we break out of the inner loop by setting "found" to TRUE. */
  for ( row=0 ; row < N ; row++ ) {
    found = FALSE;
    for ( col=0 ; (col < N) && !found ; col++ ) {
      if ( Legal_Pos (row, col, rows) ) {
	rows[row] = col;
	found = TRUE;
      }
    }
  }
  
  /* Print out the solution we found.  No error checking to
     complain if the solution is incomplete! */

  return(1);
}



/***************************************** 
 * Function: Legal_Pos
 * Usage: if (Legal_Pos (row, col, rows)) ...
 * 
 * Checks to see if assigning a queen in column "col" of row "row"
 * is legal, given the assignments already recorded in the array "rows."
 * 
 * Input: row (the row being assigned)
 *        col (the column being assigned)
 *        rows (a length-N array with the current assignments)
 * Output: TRUE (if the assignment is legal) or FALSE (if not)
*****************************************/

int Legal_Pos (int row, int col, int rows[N]) 
{
  int i;

  /* See if there is a conflict with any other assignnemt */
  for ( i=0 ; i < N ; i++ ) {
    if ( i != row ) {          /* don't care about conflicts with
				  the current row */
      if ( rows[i] != -1 ) {   /* no conflict if the row we're checking
				  hasn't yet been assigned */
	/* There's a conflict if the row distance is the same as
	   the column distance (diagonal attack), or if the column is 
	   the same (vertical attack) */
	if ( (abs(col-rows[i]) == abs(i-row)) ||
	     (col == rows[i]) ) {
	  return(0);
	}
      }
    }
  }
  /* If no rows generated a conflict, then the assignment
     is legal. */
  return(1);
}
