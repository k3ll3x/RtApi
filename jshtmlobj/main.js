import * as THREE from 'https://cdn.skypack.dev/three@0.132.2';
import { OrbitControls } from 'https://cdn.skypack.dev/three@0.132.2/examples/jsm/controls/OrbitControls.js';
import { MTLLoader } from 'https://cdn.skypack.dev/three@0.132.2/examples/jsm/loaders/MTLLoader.js';
import { OBJLoader } from 'https://cdn.skypack.dev/three@0.132.2/examples/jsm/loaders/OBJLoader.js';

function main() {
	const canvas = document.querySelector('#c');
	const renderer = new THREE.WebGLRenderer({ antialias: true, canvas });
	renderer.outputColorSpace = THREE.SRGBColorSpace;

	const fov = 45;
	const aspect = 2;  // the canvas default
	const near = 0.1;
	const far = 1000;
	const camera = new THREE.PerspectiveCamera(fov, aspect, near, far);
	camera.position.set(0, 10, 20);

	const controls = new OrbitControls(camera, canvas);
	controls.target.set(0, 5, 0);
	controls.update();

	const scene = new THREE.Scene();
	scene.background = new THREE.Color('lightblue');

	{
		const mtlLoader = new MTLLoader();
		mtlLoader.load("nau.mtl", (materials) => {
			materials.preload();
			var objLoader = new OBJLoader();
			objLoader.setMaterials(materials);
			objLoader.load('nau.obj', (object) => {
				object.position.y = - 95;
				scene.add(object);
			});
		});
	}

	{
		const light = new THREE.HemisphereLight(0x0000ff, 0x00ff00, 0.6);
		scene.add(light);
	}

	// {
	// 	const skyColor = 0xB1E1FF;  // light blue
	// 	const groundColor = 0xB97A20;  // brownish orange
	// 	const intensity = 1;
	// 	const light = new THREE.HemisphereLight(skyColor, groundColor, intensity);
	// 	scene.add(light);
	// }

	{
		const color = 0xFFFFFF;
		const intensity = 1;
		const light = new THREE.DirectionalLight(color, intensity);
		light.position.set(5, 10, 2);
		scene.add(light);
		scene.add(light.target);
	}

	function resizeRendererToDisplaySize(renderer) {
		const canvas = renderer.domElement;
		const width = canvas.clientWidth;
		const height = canvas.clientHeight;
		const needResize = canvas.width !== width || canvas.height !== height;
		if (needResize) {
			renderer.setSize(width, height, false);
		}
		return needResize;
	}

	function render() {

		if (resizeRendererToDisplaySize(renderer)) {
			const canvas = renderer.domElement;
			camera.aspect = canvas.clientWidth / canvas.clientHeight;
			camera.updateProjectionMatrix();
		}

		renderer.render(scene, camera);

		requestAnimationFrame(render);
	}

	requestAnimationFrame(render);
}

main();