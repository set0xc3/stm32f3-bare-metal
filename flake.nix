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

        # Правило для устройства ST-LINK/V2.1
        echo 'SUBSYSTEM=="usb", ATTR{idVendor}=="0483", ATTR{idProduct}=="374b", MODE="0666"'>/etc/udev/rules.d/99-st-link.rules
        udevadm control --reload-rules
      '';
    };
  };
}
