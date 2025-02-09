import { readFile } from "fs";
import { hilbertCurveSort2D } from "./hilbert_curve_sort.js";

function calcDist([x1, y1], [x2, y2], round) {
	return round(Math.sqrt((x1 - x2) ** 2 + (y1 - y2) ** 2));
}

function main() {
	readFile("pla85900.tsp", async (err, data) => {
		if (err) throw err;

		const pla85900 = data.toString();
		const lines = pla85900.split("\n");
		const points = [];
		for (let i = 6; i < lines.length - 1; i++) {
			points.push(lines[i].split(/\s+/).slice(1).map(Number));
		}

		const start = performance.now();
		const sorted = hilbertCurveSort2D(points);
		const stop = performance.now();
		const duration = stop - start;

		let dist = 0;
		for (let i = 0; i < sorted.length; i++) {
			dist += calcDist(sorted[i], sorted[(i + 1) % sorted.length], Math.ceil);
		}
		console.log(`Sorted in ${duration}ms. Dist: ${dist}, ${dist / 142382641}x optimal.`);
		// Sorted in (~400ms). Dist: 188465250, 1.3236532815822681x optimal.
		// 11th Gen Intel(R) Core(TM) i5-11320H
	});
}

main();
