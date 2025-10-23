use std::time::Instant;

#[derive(Debug, Clone)]
pub struct Event {
    pub label: String,
    pub milliseconds: u128,
}

#[derive(Debug, Clone)]
pub struct Stopwatch {
    start: Instant,
    last: Instant,
    events: Vec<Event>,
}

impl Stopwatch {
    pub fn new() -> Self {
        let now = Instant::now();
        Self {
            start: now,
            last: now,
            events: Vec::new(),
        }
    }

    pub fn tick<S: Into<String>>(&mut self, label: S) -> u128 {
        let now = Instant::now();
        let elapsed = now.duration_since(self.last).as_millis();
        self.last = now;
        let event = Event {
            label: label.into(),
            milliseconds: elapsed,
        };
        self.events.push(event.clone());
        elapsed
    }

    pub fn events(&self) -> &[Event] {
        &self.events
    }

    pub fn total_ms(&self) -> u128 {
        Instant::now().duration_since(self.start).as_millis()
    }
}
