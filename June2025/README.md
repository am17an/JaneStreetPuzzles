This is a nice and satisfying puzzle. 

First hint in the letters on top and the left of the board. They spell out PAR-TRI-DGE TIL-ING. A quick google search would lead to thepartridge puzzle which is tells us that we can tile this board using the following identity.

$$ \sum_{i=1}^{N} i^3 = \frac{N(N+1)}{2}^2 $$

Here N = 9, so we get a board which is 45x45, which contains of 9 9x9 squares, 8 8x8 squares ... and 1 1x1 square. (Some ones in the puzzle name should be clue that this may be important)

After that it's a tedious task to taking these half complete boards are solving them. Since the board is almost complete, we can just backtrack to get the answer for all 9 boards. The annoying part is the manual OCR as I could not find a LLM to do this reliably. 

Now if you notice the placement of letters on the board is telling, namely the distance between letters is the distance in the alphabet modulo 45 i.e. the alphabet wraps around. 

Taking the position of the 1x1 boards and getting their row, col letters we get the answer

>! The sum of squares is a cube
