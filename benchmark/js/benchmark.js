import { readFile } from "node:fs/promises";
import { sort2D } from "./hilbert_curve_sort.js";

function calcDist([x1, y1], [x2, y2]) {
	return Math.ceil(Math.sqrt((x1 - x2) ** 2 + (y1 - y2) ** 2));
}

const points = [];

const pla85900 = await readFile("pla85900.tsp", "ascii");
const lines = pla85900.split("\n");

for (let i = 6; i < lines.length - 1; i++) {
	points.push(lines[i].split(/\s+/).slice(1).map(Number));
}

const start = performance.now();

const sorted = sort2D(points);

const stop = performance.now();

const duration = stop - start;

let dist = 0;

for (let i = 0; i < sorted.length - 1; i++) {
	dist += calcDist(sorted[i], sorted[i + 1]);
}

dist += calcDist(sorted.at(-1), sorted[0]);

console.log("time:", duration);
console.log("dist:", dist);
console.log("mult:", (dist / 142382641).toFixed(6));

// time: 170.
// dist: 188465250.
// mult: 1.323653.
// 11th Gen Intel(R) Core(TM) i5-11320H @ 3.20GHz
