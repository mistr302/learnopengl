{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs =
    {
      self,
      nixpkgs,
      ...
    }@inputs:
    let
      eachSystem =
        f:
        nixpkgs.lib.genAttrs nixpkgs.lib.systems.flakeExposed (system: f nixpkgs.legacyPackages.${system});
    in
    {
      # packages = eachSystem (pkgs: {
      # });

      devShells = eachSystem (pkgs: {
        default = pkgs.mkShell (
          with pkgs;
          {
            buildInputs = [
              gcc
              libglvnd
              libglvnd.dev
              python3Packages.glad # or python3Packages.glad2 for newer version
              glfw
              gdb
              glib
            ];
          }
        );
      });

    };
}
