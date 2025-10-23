from __future__ import annotations

from dataclasses import dataclass
from time import perf_counter
from typing import List


@dataclass(frozen=True)
class Event:
    label: str
    milliseconds: int


class Stopwatch:
    def __init__(self) -> None:
        now = perf_counter()
        self._start = now
        self._last = now
        self._events: List[Event] = []

    def tick(self, label: str) -> int:
        now = perf_counter()
        elapsed_ms = int((now - self._last) * 1000)
        self._events.append(Event(label, elapsed_ms))
        self._last = now
        return elapsed_ms

    @property
    def events(self) -> List[Event]:
        return list(self._events)

    @property
    def total_ms(self) -> int:
        return int((self._last - self._start) * 1000)
