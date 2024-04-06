{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs = {
    self,
    nixpkgs,
    ...
  } @ inputs: let
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages.${system};
  in {
    devShells.${system}.default = pkgs.mkShell {
      buildInputs = [
        pkgs.bear
        pkgs.gdb
        pkgs.usbutils
        pkgs.minicom
        pkgs.gcc-arm-embedded
        pkgs.stlink-gui
      ];

      shellHook = ''
        exec $SHELL
      '';
    };
  };
}
