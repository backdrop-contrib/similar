<?php

/**
 * @file
 * Provides a field for displaying the similarity score of similar entries.
 */

/**
 * Similarity score field handler.
 */
class similar_handler_field_similarity extends views_handler_field {

  /**
   * Option definition.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['score_type'] = array('default' => 1);
    $options['percent_suffix'] = array('default' => 1);
    return $options;
  }

  /**
   * Options form for selecting between number and percentage.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['score_type'] = array(
      '#type' => 'radios',
      '#title' => t('Display type'),
      '#default_value' => $this->options['score_type'],
      '#options' => array(
        0 => t('Show similarity score'),
        1 => t('Show as percentage'),
      ),
    );
    $form['percent_suffix'] = array(
      '#type' => 'checkbox',
      '#title' => t('Append % when showing percentage'),
      '#default_value' => !empty($this->options['percent_suffix']),
    );    
  }

  function query() {}

  /**
   * Renders the score as a number or a percentage that cannot exceed 50.
   */
  function render($values) {
    if ($this->options['score_type'] == 0) {
      $score = $values->score / 10;
      return $score > 50 ? 50 : $score;
    }
    else {
      $score = round($values->score / 10);
      $output = $score > 50 ? 50 : $score;
      if (!empty($this->options['percent_suffix'])) {
        $output .= '%';
      }
      return $output;
    }
    
  }
}