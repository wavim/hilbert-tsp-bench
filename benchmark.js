function main() {
	const fs = require("fs");
	fs.readFile("pla85900.tsp", async (err, data) => {
		if (err) throw err;

		const pla85900 = data.toString();
		const lines = pla85900.split("\n");
		const coords = [];
		for (let i = 6; i < lines.length - 1; i++) coords.push(lines[i].split(/\s+/).slice(1).map(Number));

		let sorted = await h2CurveSort(coords);
		sorted.push(sorted[0]);

		let dist = 0;
		for (let i = 0; i < sorted.length - 1; i++) dist += calcDist(sorted[i], sorted[i + 1], Math.ceil);
		console.log(`hilbert curve dist: ${dist}, ${dist / 142382641} times optimal`);
		// hilbert curve dist: 188465250, 1.3236532815822681 times optimal
	});
}

// Gray Code defining orthants' order
const grayCode = (n) => [...Array(2 ** n).keys()].map((bit) => bit ^ (bit >> 1));
const GRAY_2 = grayCode(2);

async function h2CurveSort(vec2s) {
	let [minX, minY] = ([maxX, maxY] = vec2s[0]);
	for (let i = 1; i < vec2s.length; i++) {
		const [x, y] = vec2s[i];
		if (x < minX) minX = x;
		else if (x > maxX) maxX = x;
		if (y < minY) minY = y;
		else if (y > maxY) maxY = y;
	}
	const sideX = maxX - minX;
	const sideY = maxY - minY;
	const maxSide = Math.max(sideX, sideY);

	// Centering and scaling data to fit hypercube
	let scaleX = maxSide / sideX;
	let scaleY = maxSide / sideY;
	if (scaleX === 0 || !Number.isFinite(scaleX)) scaleX = 1;
	if (scaleY === 0 || !Number.isFinite(scaleY)) scaleY = 1;
	const normVec2s = vec2s.map(([x, y]) => [scaleX * (x - minX), scaleY * (y - minY)]);

	// De-scaling and de-centering the results from actual sort
	return (await _h2CurveSort(normVec2s, maxSide)).map(([x, y]) => [x / scaleX + minX, y / scaleY + minY]);
}

async function _h2CurveSort(vec2s, side) {
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

	const quads = [...Array(4)].map(() => []);
	for (const vec2 of vec2s) {
		const bitX = vec2[0] > mid;
		const bitY = vec2[1] > mid;
		// Quadrant as G_2 bit
		const quad = (bitX << 1) + bitY;
		quads[quad].push(vec2);
	}

	// Transform quadrants to U_2(1)
	const sorted = await Promise.all(
		quads.map(async (quadVec2s, quad) => await _h2CurveSort(quadVec2s.map(maps[quad]), mid)),
	);
	// Order quadrants w.r.t G_2 and de-transform
	return GRAY_2.flatMap((quad) => sorted[quad].map(inverseMaps[quad]));
}

function calcDist([x1, y1], [x2, y2], round) {
	return round(Math.sqrt((x1 - x2) ** 2 + (y1 - y2) ** 2));
}

main();
