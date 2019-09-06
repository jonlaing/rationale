exception ReadError(string);
type envT = {
  path: string,
  dir: string,
  target: string
};
let env = {path: "./input.txt", dir: "/", target: "./out.txt"};

let readFile = KIO.lift(env => {
    try (Node.Fs.readFileSync(env.path, `ascii)) {
    | ReadError(msg) => raise @@ ReadError("File read failed: " ++ msg)
    };
  });

let saveFile = str => KIO.lift(env => {
  Node.Fs.writeFileSync(env.target, str, `ascii);
});

let log = (x: string) =>
  KIO.return({
    Js.log(x)
  });

let parseFile = input => {
  let l  = Js.String.split("\n", input);
  Array.map(x => x ++ "100", l);
} 

let joinArray = (xs: array(string)) => Js.Array.join(xs)

let main = KIO.(
  readFile 
    <$> parseFile
    <$> joinArray
    >>= saveFile
)

KIO.runIO(main, env);
