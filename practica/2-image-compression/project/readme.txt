# Verslag Image Compression

- Wout Coenen - 1334370
- Reinaert Van de Cruys - 1334947


## Implementatie

Volgens ons hebben we **alle gevraagde functionaliteiten geïmplementeerd**, wat
inhoudt:

1. **Opdelen** in 4x4 pixel blokken
2. **DCT** op de pixel blokken toepassen
3. **Kwantiseren** van de pixel blokken volgens de gegeven matrix
4. **Zig-zag** scannen van de gekwantiseerde coëfficiënten
5. **RLE** toepassen wanneer gevraagd
6. **Opslaan** in een compact formaat dat alle nodige informatie bevat om de
   afbeelding weer op te bouwen (zie `storage_model.txt` voor meer details)

Verder wordt een **gedetailleerde log** uitgeschreven naar het gegeven pad,
zoals gevraagd, en wordt ook een beperkte log uitgeschreven naar de console.


## Artefacten

We hebben onze codec op een 20-tal afbeeldingen getest (waaronder randgevallen
zoals effen kleuren en ruis), en **onze codec creëert geen abnormale
artefacten** voor zover we konden bepalen.
