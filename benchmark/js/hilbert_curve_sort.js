function minmax(vecs, i) {
	let min = Infinity;
	let max = -Infinity;

	for (const item of vecs) {
		const value = item[i];

		if (value < min) {
			min = value;
			continue;
		}

		if (value > max) {
			max = value;
		}
	}

	return [min, max];
}

export function sort2D(vec2s) {
	const [minx, maxx] = minmax(vec2s, 0);
	const [miny, maxy] = minmax(vec2s, 1);

	const sidex = maxx - minx;
	const sidey = maxy - miny;
	const bound = Math.max(sidex, sidey);

	const scalex = bound === 0 || sidex === 0 ? 1 : bound / sidex;
	const scaley = bound === 0 || sidey === 0 ? 1 : bound / sidey;

	// Centering and scaling data to fit hypercube
	const fitted = vec2s.map(([x, y]) => [scalex * (x - minx), scaley * (y - miny)]);

	// De-scaling and de-centering the results from actual sort
	return runSort2D(fitted, bound).map(([x, y]) => [x / scalex + minx, y / scaley + miny]);
}

// Gray Code defining orthants' order
function gray(n) {
	return [...Array(1 << n).keys()].map((bit) => bit ^ (bit >> 1));
}

function runSort2D(vec2s, side) {
	if (isBase2D(vec2s)) {
		return vec2s;
	}

	const mid = side / 2;

	// Transformations of quadrants to (from) U_2(1)
	const maps = {
		0b00: ([x, y]) => [y, x],
		0b01: ([x, y]) => [x, y - mid],
		0b11: ([x, y]) => [x - mid, y - mid],
		0b10: ([x, y]) => [mid - y, side - x],
	};
	const invs = {
		0b00: ([x, y]) => [y, x],
		0b01: ([x, y]) => [x, y + mid],
		0b11: ([x, y]) => [x + mid, y + mid],
		0b10: ([x, y]) => [side - y, mid - x],
	};

	const quads = [[], [], [], []];

	for (const vec2 of vec2s) {
		const bitx = +(vec2[0] > mid);
		const bity = +(vec2[1] > mid);

		// Quadrant as G_2 bit
		const quad = (bitx << 1) + bity;
		quads[quad].push(vec2);
	}

	// Transform quadrants to U_2(1)
	const sorted = quads.map((vec2s, quad) => runSort2D(vec2s.map(maps[quad]), mid));

	// Order quadrants w.r.t G_2 and inv-transform
	return gray(2).flatMap((quad) => sorted[quad].map(invs[quad]));
}

function isBase2D(vec2s) {
	if (vec2s.length < 2) {
		return true;
	}

	const first = vec2s[0];

	return vec2s.slice(1).every((vec2) => {
		return vec2[0] === first[0] && vec2[1] === first[1];
	});
}
