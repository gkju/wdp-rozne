import {promises as fs} from 'fs';


for(let i = 0; i < 1000; ++i) {
    let n = Math.floor(Math.random() * 1000 + 1);
    let output = `${n}\n`;
    let prev = 0;
    for(let j = 0; j < n; ++j) {
        let next = prev + Math.floor(Math.random() * 1000 + 1);
        output += `${Math.floor(Math.random() * 3 + 1)} ${next}\n`;
        prev = next; 
    }
    fs.writeFile(`./tests/trz_${i}.in`, output);
}