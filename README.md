## PEARL: Power- and Energy-Aware Multicore Intermittent Computing

This is the repo associated with the paper [PEARL: Power- and Energy-Aware Multicore Intermittent Computing](ewsn25-final20.pdf), presented at the [EWSN'2025](https://www.ewsn25.cs.kuleuven.be/) Conference in Leuven, Belgium. 

**Abstract** *Low-power multicore platforms are suitable for running data-intensive tasks in parallel, but they are highly inefficient for computing on intermittent power. In this work, we present PEARL (PowEr And eneRgy-aware MuLticore Intermittent Computing), a novel systems support that can make existing multicore microcontroller (MCU) platforms suitable for efficient intermittent computing. PEARL achieves this by leveraging only a three-threshold voltage tracking circuit and an external fast non-volatile memory, which multicore MCUs can smoothly interface with. PEARL software runtime manages these components and performs energy- and power-aware adaptation of the multicore configuration to introduce minimal backup overheads and boost performance. Our evaluation shows that PEARL outperforms the state-of-the-art solutions by up to 30× and consumes up to 32× less energy.*

## Content of the Project

- [PEARL MAX32666 Implementation](MAX32666_project/)
- [PEARL Python Simulations](Python_sim/)

### To cite the paper and code:

```bibtex
@inproceedings{akhunov2025pearl,
      title={PEARL: Power- and Energy-Aware Multicore Intermittent Computing}, 
      author={Khakim Akhunov, Eren Yildiz and Kasim Sinan Yildirim},
      year={2025},
      booktitle={The 22nd International Conference on Embedded Wireless Systems and Networks (EWSN'25)}
}
```

### Acknowledgments

<a href="https://www.unitn.it/"><img src="img/unitn_logo.png" width="300px"></a> 

### Copyright

MIT Licence or otherwise specified. See [license](./LICENSE.txt) file for details.
