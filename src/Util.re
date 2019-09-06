let eq = (a, b) => a == b;

let eqBy = (f, a, b) => eq(f(a), f(b));

let identical = (a, b) => a === b;

let idCounter = ref(0);
let uniqueId = (~prefix="", ()) => {
  idCounter := idCounter^ + 1;
  prefix ++ string_of_int(idCounter^);
};
