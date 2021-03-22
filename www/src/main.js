function generateGraphs() {
	let data = document.getElementById('data').value;
	let lines = data.split('\n');
	lines.shift();

	let info = {
		date: [],
		ti: [],
		tr: [],
		te: [],
		fan: [],
		resistor: []
	};

	lines.forEach((line) => {
		let rows = line.split(',');
		
		info.date.push(rows[0]);
		info.ti.push(rows[1]);
		info.tr.push(rows[2]);
		info.te.push(rows[3]);
		info.fan.push(rows[4]);
		info.resistor.push(rows[5]);
	});
	
	let optionsOne = {
		chart: {
			type: 'line',
			toolbar: {
				export: {
					csv: {
						filename: 'temperatures'
					},
					
					png: {
						filename: 'temperatures'
					},
					
					svg: {
						filename: 'temperatures'
					}
				}
			}
		},

		series: [
			{
				name: 'TI',
				data: info.ti
			},
			
			{
				name: 'TR',
				data: info.tr
			},
			
			{
				name: 'TE',
				data: info.te
			}
		],

		xaxis: {
			categories: info.date,
			labels: {
				show: false
			}
		},
		
		legend: {
			show: false
		}
	};
	
	let optionsTwo = {
		chart: {
			type: 'line',
			toolbar: {
				export: {
					csv: {
						filename: 'actuators'
					},
					
					png: {
						filename: 'actuators'
					},
					
					svg: {
						filename: 'actuators'
					}
				}
			}
		},

		series: [
			{
				name: 'Fan',
				data: info.fan
			},
			
			{
				name: 'Resistor',
				data: info.resistor
			}
		],

		xaxis: {
			categories: info.date,
			labels: {
				show: false
			}
		},
		
		legend: {
			show: false
		},
		
		stroke: {
			curve: 'stepline'
		}
	};
	
	let chartOne = new ApexCharts(document.querySelector("#chartOne"), optionsOne);
	let chartTwo = new ApexCharts(document.querySelector("#chartTwo"), optionsTwo);
	
	chartOne.render();
	chartTwo.render();
}