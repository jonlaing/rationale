open Belt.Result;

let fail = err => Error(err);

let isOk = r =>
  switch (r) {
  | Ok(_) => true
  | _ => false
  };

let getOk = r =>
  switch (r) {
  | Ok(a) => Some(a)
  | _ => None
  };

let getError = r =>
  switch (r) {
  | Error(err) => Some(err)
  | _ => None
  };

let isError = r =>
  switch (r) {
  | Error(_) => true
  | _ => false
  };

let ofOption = (err, r) =>
  switch (r) {
  | Some(a) => Ok(a)
  | None => Error(err)
  };

let ap = (r, a) =>
  switch (r) {
  | Ok(f) => Ok(f(a))
  | Error(err) => Error(err)
  };

let result = (okF, errF, r) =>
  switch (r) {
  | Ok(a) => okF(a)
  | Error(err) => errF(err)
  };

let bimap = (okF, errF, r) =>
  switch (r) {
  | Ok(a) => Ok(okF(a))
  | Error(err) => Error(errF(err))
  };

include Monad.MakeBasic2({
  type t('a, 'd) = Belt.Result.t('a, 'd);
  let bind = (r, f) =>
    switch (r) {
    | Ok(a) => f(a)
    | Error(err) => Error(err)
    };
  let return = a => Ok(a);
  let fmap = `DefineWithBind;
});

module Infix = {
  let (>>=) = (>>=);
  let (<$>) = (<$>);
};