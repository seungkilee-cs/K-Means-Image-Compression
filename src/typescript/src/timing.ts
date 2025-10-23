export interface Event {
  label: string;
  milliseconds: number;
}

export class Stopwatch {
  private start: number;
  private last: number;
  private events: Event[] = [];

  constructor() {
    const now = performance.now();
    this.start = now;
    this.last = now;
  }

  tick(label: string): number {
    const now = performance.now();
    const elapsed = now - this.last;
    this.last = now;
    const event = { label, milliseconds: Math.round(elapsed) };
    this.events.push(event);
    return event.milliseconds;
  }

  getEvents(): Event[] {
    return this.events.slice();
  }

  totalMs(): number {
    return Math.round(performance.now() - this.start);
  }
}
