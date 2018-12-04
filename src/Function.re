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

let debounce = (~immediate=false, wait, f) => Js.Global.({
  let timeout = ref(None);
  let afterWait = f => timeout := setTimeout(f, wait)->Some;
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
});