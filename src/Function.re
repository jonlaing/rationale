let always = (x, _y) => x;

let compose = (f, g, x) => f(g(x));

let pipe = (f, g, x) => g(f(x));

let false_ = x => always(false, x);

let true_ = x => always(true, x);

let flip = (f, a, b) => f(b, a);

let identity = x => x;

module Infix = {
  let (<||) = compose;
  let (||>) = pipe;
};

include Infix;

let delay = Js.Global.setTimeout;

let debounce = (~immediate=false, f, wait) => {
  open Js.Global;
  let timeout = ref(None);
  let afterWait = f => timeout := delay(f, wait)->Some;
  () =>
    switch (immediate, timeout^) {
    | (true, None) =>
      f();
      afterWait(() => timeout := None);
    | (true, Some(timerId)) =>
      clearTimeout(timerId);
      afterWait(() => timeout := None);
    | (false, None) =>
      afterWait(() => {
        f();
        timeout := None;
      })
    | (false, Some(timerId)) =>
      clearTimeout(timerId);
      afterWait(() => {
        f();
        timeout := None;
      });
    };
};

let throttle = (f, wait) => {
  open Js.Global;
  let timeout = ref(None);
  let previous = ref(0.0);
  () =>
    switch (timeout^) {
    | None =>
      f();
      timeout := delay(() => timeout := None, wait)->Some;
      previous := Js.Date.now();
    | Some(timerId) =>
      clearTimeout(timerId);
      let now = Js.Date.now();
      let remaining = wait - int_of_float(now -. previous^);
      if (remaining <= 0 || remaining > wait) {
        f();
        previous := Js.Date.now();
      } else {
        timeout :=
          delay(
            () => {
              f();
              previous := Js.Date.now();
            },
            remaining,
          )
          ->Some;
      };
    };
};