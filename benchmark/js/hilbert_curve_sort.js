// Gray Code defining orthants' order
const GRAY_CODE = (n) => [...Array(1 << n).keys()].map((bit) => bit ^ (bit >> 1));
const GRAY_2 = GRAY_CODE(2);

export function hilbertCurveSort2D(vec2s) {
	let [minX, minY] = [Infinity, Infinity];
	let [maxX, maxY] = [-Infinity, -Infinity];
	for (const [x, y] of vec2s) {
		[minX, minY] = [Math.min(x, minX), Math.min(y, minY)];
		[maxX, maxY] = [Math.max(x, maxX), Math.max(y, maxY)];
	}
	const sideX = maxX - minX;
	const sideY = maxY - minY;
	const maxSide = Math.max(sideX, sideY);

	// Centering and scaling data to fit hypercube
	const scaleX = maxSide === 0 || sideX === 0 ? 1 : maxSide / sideX;
	const scaleY = maxSide === 0 || sideY === 0 ? 1 : maxSide / sideY;
	const fitVec2s = vec2s.map(([x, y]) => [scaleX * (x - minX), scaleY * (y - minY)]);

	// De-scaling and de-centering the results from actual sort
	return runHilbertCurveSort2D(fitVec2s, maxSide).map(([x, y]) => [x / scaleX + minX, y / scaleY + minY]);
}

function runHilbertCurveSort2D(vec2s, side) {
	// Base
	if (vec2s.length < 2 || new Set(vec2s.map(String)).size === 1) return vec2s;

	// Recursion
	const mid = side / 2;
	// Transformation of quadrants to (from) U_2(1)
	const maps = {
		0b00: ([x, y]) => [y, x],
		0b01: ([x, y]) => [x, y - mid],
		0b11: ([x, y]) => [x - mid, y - mid],
		0b10: ([x, y]) => [mid - y, side - x],
	};
	const inverseMaps = {
		0b00: ([x, y]) => [y, x],
		0b01: ([x, y]) => [x, y + mid],
		0b11: ([x, y]) => [x + mid, y + mid],
		0b10: ([x, y]) => [side - y, mid - x],
	};

	const quads = [[], [], [], []];
	for (const vec2 of vec2s) {
		const bitX = vec2[0] > mid;
		const bitY = vec2[1] > mid;
		// Quadrant as G_2 bit
		const quad = (bitX << 1) + bitY;
		quads[quad].push(vec2);
	}

	// Transform quadrants to U_2(1)
	const sorted = quads.map((quadVec2s, quad) => runHilbertCurveSort2D(quadVec2s.map(maps[quad]), mid));
	// Order quadrants w.r.t G_2 and de-transform
	return GRAY_2.flatMap((quad) => sorted[quad].map(inverseMaps[quad]));
}
