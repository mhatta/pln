;; Kownledge base for the amusing friend demo.
;;
;; We are dodging a lot of representational issues here. In particular
;; everything related to spatio-temporal reasoning. To be reified as
;; desired.

;; Bob is a human
(Inheritance (stv 1 1)
   (Concept "Bob")
   (Concept "human"))

;; I am a human
(Inheritance (stv 1 1)
   (Concept "Self")
   (Concept "human"))

;; I know Bob
(Evaluation (stv 1 1)
   (Predicate "acquainted")
   (List
      (Concept "Self")
      (Concept "Bob")))

;; The probablity of turning acquaintance into friendship between
;; humans is 0.1.
(Implication (stv 0.1 0.5)
   (VariableList
      (TypedVariable
         (Variable "$X")
         (Type "ConceptNode"))
      (TypedVariable
         (Variable "$Y")
         (Type "ConceptNode")))
   (And
      (Inheritance
         (Variable "$X")
         (Concept "human"))
      (Inheritance
         (Variable "$Y")
         (Concept "human"))
      (Evaluation
         (Predicate "acquainted")
         (List
            (Variable "$X")
            (Variable "$Y"))))
   (Evaluation
      (Predicate "will-be-friends")
      (List
         (Variable "$X")
         (Variable "$Y"))))

;; Friends tend to be honest
(Implication (stv 0.6 0.5)
   (VariableList
      (TypedVariable
         (Variable "$X")
         (Type "ConceptNode"))
      (TypedVariable
         (Variable "$Y")
         (Type "ConceptNode")))
   (Evaluation
      (Predicate "are-friends")
      (List
         (Variable "$X")
         (Variable "$Y")))
   (And
      (Evaluation
         (Predicate "is-honest")
         (Variable "$X"))
      (Evaluation
         (Predicate "is-honest")
         (Variable "$Y"))))

;; Friends will remain friends.
;;
;; This could simply be expressed as
;;
;; (Implication (stv 0.9 0.9)
;;    (Predicate "are-friends")
;;    (Predicate "will-be-friends"))
;;
;; but due to some current limitation in the type system, specifically
;; that a Predicate cannot be declared with a certain type, we need to
;; express that in a more convoluted way.
(Implication (stv 0.9 0.9)
   (VariableList
      (TypedVariable
         (Variable "$X")
         (Type "ConceptNode"))
      (TypedVariable
         (Variable "$Y")
         (Type "ConceptNode")))
   (Evaluation
      (Predicate "are-friends")
      (List
         (Variable "$X")
         (Variable "$Y")))
   (Evaluation
      (Predicate "will-be-friends")
      (List
         (Variable "$X")
         (Variable "$Y"))))

;; Bob told Jill the truth about the party
(Evaluation
   (Predicate "told-the-truth-about")
   (List
      (Concept "Bob")
      (Concept "Jill")
      (Concept "Party")))

;; People who told the truth about something are honest
(Implication (stv 0.8 0.9)
   (VariableList
      (TypedVariable
         (Variable "$X")
         (Type "ConceptNode"))
      (TypedVariable
         (Variable "$Y")
         (Type "ConceptNode"))
      (TypedVariable
         (Variable "$Z")
         (Type "ConceptNode")))
   (Evaluation
      (Predicate "told-the-truth-about")
      (List
         (Variable "$X")
         (Variable "$Y")
         (Variable "$Z")))
   (Evaluation
      (Predicate "is-honest")
      (Variable "$X")))

;; Bob told Jim a joke at the party.
(Evaluation    
   (Predicate "told-a-joke-at")
      (List
         (Concept "Bob")
         (Concept "Jim")
         (Concept "Party")))

;; People who told a joke to someone, somewhere, are funny   
(Implication (stv 0.8 0.9)
   (VariableList
      (TypedVariable
         (Variable "$X")
         (Type "ConceptNode"))
      (TypedVariable
         (Variable "$Y")
         (Type "ConceptNode"))
      (TypedVariable
         (Variable "$Z")
         (Type "ConceptNode")))
   (Evaluation
      (Predicate "told-a-joke-at")
      (List
         (Variable "$X")
         (Variable "$Y")
         (Variable "$Z")))
   (Evaluation
      (Predicate "is-funny")
      (Variable "$X")))
 
;; Being funny is loosely equivalent to being amusing
;;
;; This could simply be expressed as
;;
;; (Equivalence (stv 0.7 0.9)
;;    (Predicate "is-funny")
;;    (Predicate "is-amusing"))
;;
;; but due to some current limitation in the type system, specifically
;; that a Predicate cannot be declared with a certain type, we need to
;; express that in a more convoluted way.
(Equivalence (stv 0.7 0.9)
   (TypedVariable
      (Variable "$X")
      (Type "ConceptNode"))
   (Evaluation
      (Predicate "is-funny")
      (Variable "$X"))
   (Evaluation
      (Predicate "is-amusing")
      (Variable "$X")))
