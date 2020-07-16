with import <nixpkgs> {};
stdenv.mkDerivation {
  name = "game-of-life";
  nativeBuildInputs = [ cmake ];
  buildInputs = [
    SDL2
  ];
}
