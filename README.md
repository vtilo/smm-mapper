<p align="center">
    <h3 align="center">SmmMapper</h3>
    <p align="center"><i>UEFI SMM payload mapper with hot reload for on-hardware development</i></p>
</p>

## About

I made this project because working on SMM drivers directly on hardware is problematic. Instead of having to reflash your SPI chip every time you want to test a new version of your module, now you can just map your SMM payloads through an SMM host and swap them out from disk or even hot reload them from Windows

<p align="center">
  <img width="720" alt="SmmMapper overview" src="https://github.com/user-attachments/assets/0413524f-7fe0-4385-95cb-d5611c6504ad" />
</p>

## Support

* x64 UEFI firmware with AMI Aptio V style PI SMM support
* Windows 10 or Windows 11 for the Windows client
* Tested on ASUS TUF X870 with an AMD AM5 platform (Should also work on Intel)

## Usage

1. Open an [x64 Visual Studio developer command prompt](https://learn.microsoft.com/en-us/visualstudio/ide/reference/command-prompt-powershell?view=visualstudio) and run `src\build.cmd`
2. Add `DxeBridge.efi` and `SmmHost.efi` to your firmware and flash it onto your board. Easiest way is to just replace existing DXE and SMM modules (check out the [general UEFITool guide](https://winraid.level1techs.com/t/guide-howto-extract-insert-replace-efi-bios-modules/32122))
3. Drop your payload at `\EFI\SMM\PAYLOAD.EFI` on something UEFI can read at boot, like the EFI partition or a FAT32 USB stick
4. Boot up your target and watch the serial output for `DXE init`, `SMM init`, and payload load messages. You’ll need a COM port reader hooked up to the serial header
5. From Windows, use `SmmClient.exe` to reload, unload, ping, or check status

Note about payloads: SmmHost handles mapping on its own, not UEFI image loader. That means you should avoid using imports entirely, since they won't get resolved. If your payload has any absolute addresses, make sure to use relocations

If you are not sure how this project works or how to use it, just open an issue and I will help you out
