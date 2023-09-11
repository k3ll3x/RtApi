use rand::prelude::*;
use std::io::{stdout, Write};
use std::thread;
use std::time::Duration;
use termion::{clear, cursor};
use colored::*;

struct Grid {
    width: usize,
    height: usize,
    cells: Vec<Vec<bool>>,
}

impl std::fmt::Display for Grid {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        for row in &self.cells {
            for cell in row {
                if *cell {
                    write!(f, "{}", "█".red())?;
                } else {
                    write!(f, "{}", " ".white())?;
                }
            }
            write!(f, "\n")?;
        }
        Ok(())
    }
}

impl Grid {
    fn update(&mut self) {
        let mut new_cells = vec![vec![false; self.width]; self.height];
        for i in 0..self.height {
            for j in 0..self.width {
                let mut neighbors = 0;
                for di in -1..=1 {
                    for dj in -1..=1 {
                        if di == 0 && dj == 0 {
                            continue;
                        }
                        let ni = (i as isize + di) as usize % self.height;
                        let nj = (j as isize + dj) as usize % self.width;
                        if self.cells[ni][nj] {
                            neighbors += 1;
                        }
                    }
                }
                if self.cells[i][j] {
                    if neighbors == 2 || neighbors == 3 {
                        new_cells[i][j] = true;
                    }
                } else {
                    if neighbors == 3 {
                        new_cells[i][j] = true;
                    }
                }
            }
        }
        self.cells = new_cells;
    }
}

fn main() {
    let mut rng = thread_rng();
    let w = 100;
    let h = 100;
    let n = (w*h/2)+1;
    let mut grid = Grid {
        width: w,
        height: h,
        cells: (0..n)
            .map(|_| (0..n).map(|_| rng.gen_bool(0.2)).collect())
            .collect(),
    };
    loop {
        print!("{}{}", clear::All, cursor::Goto(1, 1));
        stdout().flush().unwrap();
        print!("{}", grid);
        stdout().flush().unwrap();
        grid.update();
        thread::sleep(Duration::from_millis(100));
    }
}