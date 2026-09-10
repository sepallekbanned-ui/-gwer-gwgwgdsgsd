# Build přes GitHub Actions

1. Vytvoř na GitHubu nový repozitář, ideálně Public, např. `ATS-MOZA-Blinker`.
2. Nahraj celý obsah této složky do repozitáře včetně `.github/workflows/build.yml`.
3. Na GitHubu otevři **Actions** → **Build ATS MOZA Blinker** → **Run workflow**.
4. Po doběhnutí otevři úspěšný run a v části **Artifacts** stáhni `ATS_MOZA_Blinker_Windows_x64`.
5. ZIP obsahuje `moza_ats_blinker.dll` a `MOZA_SDK.dll`.

Build běží na GitHub Windows x64 runneru s Visual Studio toolchainem; tvůj počítač tedy nemusí plugin kompilovat.
