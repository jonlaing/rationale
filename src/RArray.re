let swap = (i, j, arr) =>
  i != j ?
    {
      let temp = arr[i];
      arr[i] = arr[j];
      arr[j] = temp;
    } :
    ();

let shuffle = arr =>
  Array.iteri(
    (i, _) => {
      let j = Random.int(i + 1);
      swap(i, j, arr);
    },
    arr,
  );