{
  description = "The matlab-igraph toolbox";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs";
    nix-matlab = {
      url = "/home/voidee/clones/nix-matlab";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs = { self, nixpkgs, nix-matlab }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
    in {
      devShells.${system}.default = pkgs.mkShell {
        buildInputs = (with nix-matlab.packages.${system}; [
          matlab
          matlab-mlint
          matlab-mex
        ]) ++ (with pkgs; [
          astyle
          cmake
          ninja
          gdb
          hugo
          # igraph dependencies
          bison
          flex
          libxml2
        ]);
        shellHook = ''
          export OMP_NUM_THREADS=16
        '';
      };
    };
}
