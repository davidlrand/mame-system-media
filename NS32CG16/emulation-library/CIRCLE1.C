
main() {

    v_circle(900,900,450,3);
}

/* Use Bresenham's algorithm for circle generation */

v_circle(row,col,radius,width)
register unsigned int row,col,radius,width;
{

    register int Row, Col, p;

    Col = 0;
    Row = radius;
    
    p = 3 - 2 * radius;
    
    while (Col < Row) {
	v_vset(row + Row, col + Col,width);
	v_vset(row - Row, col + Col,width);
	v_vset(row + Row, col - Col,width);
	v_vset(row - Row, col - Col,width);
	v_hset(row + Col, col + Row,width);
	v_hset(row - Col, col + Row,width);
	v_hset(row + Col, col - Row,width);
	v_hset(row - Col, col - Row,width);
	printf("\n");
	if (p < 0)
	    p += 4 * Col + 6;
	else {
	    p += 4 * (Col - Row) + 10;
	    Row--;
	}
	Col++;
	if (Row == Col) {
	    v_vset(row + Row, col + Col,width);
	    v_vset(row - Row, col + Col,width);
	    v_vset(row + Row, col - Col,width);
	    v_vset(row - Row, col - Col,width);
	    v_hset(row + Col, col + Row,width);
	    v_hset(row - Col, col + Row,width);
	    v_hset(row + Col, col - Row,width);
	    v_hset(row - Col, col - Row,width);
	    printf("\n");
	}
    }
}


v_pset(row,col,width)
int row,col,width;
{
    printf("y = %d, x = %d\n",row,col);
}

