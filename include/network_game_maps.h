/*
 *	Author: Diego Ochando Torres
 *  Date: 19/03/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

// ------------------------------------------------------------------------- //

// 38 * 25
const int basic_map[950] = {
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,27, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2, 2,27, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2,41, 2, 2, 2, 2, 2,
	2, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0,31,30, 1, 2, 2, 1, 1, 1, 1, 1,38,34,39, 1,38,34,34, 8, 2, 2, 2,
	2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,27, 1, 1, 1, 1, 1,38,34,34,34,37, 1,36,34,37, 1, 9,35, 2, 2, 2,
	2, 0, 0, 0, 0, 0, 0,38,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,37, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,35,43, 2, 2,
	2, 2, 0, 0, 0, 0,38,37, 0, 0, 0, 0, 0, 0, 0, 0,20,28, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,35, 2, 2, 2,
	2, 2, 2, 0, 0,38,37, 0, 0, 2, 0, 2, 0, 0, 0,20,24,15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1,35, 1, 2, 2,
	2, 2, 2, 0, 0,35, 0, 0, 2, 2, 2, 2, 2, 0, 0,16,11,23,19, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1,38,37, 1, 2, 2,
	2, 2, 2, 0, 0,35, 0, 0, 2, 2, 2, 2, 2, 0,20,24,11,11,15, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 1,35, 1, 1, 1, 2,
	2, 2, 2, 0, 0,36,39, 0, 0, 2, 2, 2, 2,20,24,11,11,11,23,17,19, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1,35, 1, 1, 1, 2,
	2, 2, 2, 2, 0, 0,36,34,39, 0, 2, 2,20,24,11,11,11,11,11,11,23,19, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1,35, 1, 1, 2, 2,
	2, 0, 2, 2, 2, 2, 0, 0,36,39, 0, 2,16,11,11,11,11,11,11,11,11,23,19, 2, 2, 2, 2, 1, 1, 1, 1, 1,38,37, 1, 1, 2, 2,
	2, 0, 0, 2, 2, 2, 0, 0, 0,35, 0, 0,16,11,11,11,11,11,11,11,11,11,23,19, 2, 2, 1, 1, 1, 1, 1,38,37, 1, 1, 1, 2, 2,
	2, 0, 0, 2, 2, 2, 2, 0, 0,35, 0, 2,22,26,11,11,11,11,11,11,11,11,25,21, 2, 2, 1, 1,38,34,34,37, 1, 1, 1, 2, 2, 2,
	2, 2, 0, 0, 2, 2, 2, 2, 0,35, 0, 2, 2,22,26,11,11,11,11,11,11,11,15, 2, 2, 2, 1,38,37, 2, 1, 2, 1, 1, 1, 2, 2, 2,
	2, 0, 0, 0, 0, 0, 2, 0,38,37, 2, 2, 2, 2,22,18,26,11,11,11,11,25,21, 2, 2, 2, 1,35, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 0, 0, 0, 0, 0, 0,38,37, 0, 2, 2, 2, 2, 2, 2,22,18,26,11,25,21, 2, 2, 2, 1, 1,35, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 0, 0,38,34,34,37, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0,22,26,15, 1, 1, 1, 2, 2, 1,36,39, 1, 1, 1, 1, 2, 2, 2, 2, 2,
	2, 2, 0,38,37, 0, 0, 2, 2, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0,22,33, 1, 1, 1, 2, 2, 1, 1,36,34,39, 1, 1, 1, 2, 2, 2, 2,
	2, 2, 2,35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,29,32, 1, 1, 2, 2, 2, 1, 1, 1,36,39, 1, 1, 1, 2, 2, 2,
	2, 2,40,35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,29,32, 1, 1, 2, 2, 2, 2, 1, 1,35, 1, 1, 1, 2, 2, 2,
	2, 2, 2,35, 4, 0,38,34,34,34,34,39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,27, 1, 1, 1, 1, 1, 1, 1, 1,35, 1, 1, 1, 2, 2, 2,
	2, 2, 2, 5,34,34,37, 0, 0, 0, 0,36,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,37, 1, 1, 2, 2, 2, 2,
	2, 2, 2, 2, 2,42, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,27, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,27, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

const int detail_map[950] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 8, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,10, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,12,13,14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,12,13,14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

