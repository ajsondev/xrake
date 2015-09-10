function PqGramDistanceChildrenMean(node) {
	var c = dom_children_fun(node);

	if (c.length < 2)
		return 1.0; // Max value

	var sum = 0;
	for(var i=0; i < c.length-1; ++i)
		sum += pq_gram_distance(c[i], c[i+1]);

	return range_0_1(sum / (c.length-1));
}
