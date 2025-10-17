<a href="https://lpsc.in2p3.fr/" target="_blank"><img src="https://raw.githubusercontent.com/nyxlib/nyx-node/main/docs/img/logo_lpsc.svg" alt="LPSC" height="72" /></a>
&nbsp;&nbsp;&nbsp;&nbsp;
<a href="https://www.in2p3.fr/" target="_blank"><img src="https://raw.githubusercontent.com/nyxlib/nyx-node/main/docs/img/logo_in2p3.svg" alt="IN2P3" height="72" /></a>
&nbsp;&nbsp;&nbsp;&nbsp;
<a href="https://www.univ-grenoble-alpes.fr/" target="_blank"><img src="https://raw.githubusercontent.com/nyxlib/nyx-node/main/docs/img/logo_uga.svg" alt="UGA" height="72" /></a>

# Nyx Demo for C99

# Build instructions

Make sure that [Nyx-Node](https://github.com/nyxlib/nyx-node/) and [Nyx-Gen](https://github.com/nyxlib/nyx-gen/) are installed.

```bash
nyx-gen nyx-demo.json

rm -fr ./NYX_DEMO/build/
mkdir ./NYX_DEMO/build/
cd ./NYX_DEMO/build/

cmake ..
make
```

# Running the demo

```bash
./NYX_DEMO/build/demo
```

<div style="text-align: center;">
  <img src="https://raw.githubusercontent.com/nyxlib/nyx-node/main/docs/img/demo.png" alt="" style="width: 1250px;" />
</div>
